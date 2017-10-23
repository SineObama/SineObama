typedef struct tagPid {
    double tao;  //ϵͳ����ʱ������һ��Ϊ����ʱ���������
    double ts;  //����ʱ��
    int n;  //n=tao/ts
    double rin, yout, u;  //ϵͳ���롢ϵͳ�����PID���������
    double kp, ki, kd;  //PID����������
    double error_1, error_2;  //��ǰ1��2�ε���������PID�㷨����Ҫerror_2
    double error, perror, ierror, derror;  //���ڼ���PID���u�Ĳ���
    double den_1, den_2, den_3;  //Z������ĸ��ϵ��
    double num_1, num_2, num_3;  //Z�����ķ���ϵ��
    double yout_[2];  //ϵͳ�����ȥʱ�̵�ֵ
    CArray<double, double> u_;  //��������ȥʱ�̵����ֵ���������湫ʽ�е�u(k-2-n)��
    int size_Yout;  //Yout�Ĵ�С���㷨ѭ��������ÿ�ε����ֵ��������Yout�����������������
    CArray<double, double> Yout;  //�����������
    double maxYout, minYout;    //���ֵ�����ֵ����Сֵ
} Pid;

double m_t1, m_t2, m_k, m_tao;  //���ƶ������

//3����ʼ������
void PidInit(Pid*pid) {

    m_t1 = 1;
    m_t2 = 2.5;
    m_k = 2;

    pid->ts = 0.1;
    pid->tao = 0.6;

    pid->n = (int) (pid->tao / pid->ts + 0.5);
    pid->rin = 1;
    pid->yout = 0.0;
    pid->u = 0.0;
    pid->kp = 0.70;
    pid->ki = 0.15;
    pid->kd = 0.90;
    pid->error_1 = 0.0;
    pid->error_2 = 0.0;
    pid->error = 0.0;
    pid->perror = 0.0;
    pid->ierror = 0.0;
    pid->derror = 0.0;
    double z1, z2, b, c;
    if (m_t1 == m_t2) {
        z1 = exp(-(pid->ts / m_t1));  //e(-T/T1)z2=z1pid->num_1=0;
        pid->num_2 = m_k * (1 - z1 - pid->ts * z1 / m_t1);
        pid->num_3 = m_k * (z1 * z1 - z1 + pid->ts * z1 / m_t1);
        pid->den_1 = 1;
        pid->den_2 = -2 * z1;
        pid->den_3 = z1 * z1;
    } else {
        z1 = exp(-(pid->ts / m_t1));  //e(-T/T1)z2=exp(-(pid->ts/m_t2));//e(-T/T2)b=m_t1/(m_t2-m_t1);c=m_t2/(m_t1-m_t2);
        pid->num_1 = 0;
        pid->num_2 = m_k * (1 + b * z1 + c * z2);
        pid->num_3 = m_k * (z1 * z2 + b * z2 + c * z1);
        pid->den_1 = 1;
        pid->den_2 = -(z1 + z2);
        pid->den_3 = z1 * z2;
    }
    pid->yout_[0] = 0.0;
    pid->yout_[1] = 0.0;

    pid->u_.SetSize(pid->n + 2, 0);
    for (int i = 0; i < pid->n + 2; i++) {
        pid->u_[i] = 0.0;
    }
    pid->size_Yout = 500;

    pid->Yout.SetSize(pid->size_Yout);
    for (int i = 0; i < pid->size_Yout - 1; i++) {
        pid->Yout[i] = 0.0;
    }
//���ֵ����Сֵ��ʼ��Ϊ��һ��ֵ
    pid->maxYout = pid->Yout[0];
    pid->minYout = pid->Yout[0];
}

void control() {
    Pid pid;
    for (int k = 0; k < pid.size_Yout; k++) {
        pid.rin = 1;  //��Ծ�ź�

        pid.u = pid.kp * pid.perror + pid.ki * pid.ierror + pid.kd * pid.derror;
        //����޷�
        if (pid.u > 10) {
            pid.u = 10;
        }

        if (pid.u < -10) {
            pid.u = -10;
        }

        pid.yout = (pid.den_2) * (pid.yout_[0]) + (pid.den_3) * (pid.yout_[1])
                + (pid.num_2) * (pid.u_[pid.n])
                + (pid.num_3) * (pid.u_[pid.n + 1]);
        pid.error = (pid.rin) - (pid.yout);

        pid.perror = pid.error;

        pid.ierror = pid.ierror + pid.error * pid.ts;
        pid.derror = (pid.error - pid.error_1) / pid.ts;

        pid.error_2 = pid.error_1;
        pid.error_1 = pid.error;

        pid.yout_[1] = pid.yout_[0];
        pid.yout_[0] = pid.yout;
        for (int j = pid.n + 1; j > 0; j--) {
            pid.u_[j] = pid.u_[j - 1];
        }

        pid.u_[0] = pid.u;

        pid.Yout[k] = pid.yout;
        //�õ�������ֵ
        if (pid.Yout[k] > pid.maxYout) {
            pid.maxYout = pid.Yout[k];
        }
        //�õ���С���ֵ

        if (pid.Yout[k] < pid.minYout) {
            pid.minYout = pid.Yout[k];
        }
    }
}

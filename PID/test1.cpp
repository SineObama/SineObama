typedef struct tagPid {
    double tao;  //系统纯延时参数，一般为采样时间的整数倍
    double ts;  //采样时间
    int n;  //n=tao/ts
    double rin, yout, u;  //系统输入、系统输出、PID控制器输出
    double kp, ki, kd;  //PID控制器参数
    double error_1, error_2;  //，前1、2次的误差，在增量PID算法中需要error_2
    double error, perror, ierror, derror;  //用于计算PID输出u的参数
    double den_1, den_2, den_3;  //Z传函分母的系数
    double num_1, num_2, num_3;  //Z传函的分子系数
    double yout_[2];  //系统输出过去时刻的值
    CArray<double, double> u_;  //控制器过去时刻的输出值，就是上面公式中的u(k-2-n)等
    int size_Yout;  //Yout的大小，算法循环次数，每次的输出值计入数组Yout，用来绘制输出曲线
    CArray<double, double> Yout;  //输出数据数组
    double maxYout, minYout;    //输出值的最大值和最小值
} Pid;

double m_t1, m_t2, m_k, m_tao;  //控制对象参数

//3、初始化参数
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
//最大值和最小值初始化为第一个值
    pid->maxYout = pid->Yout[0];
    pid->minYout = pid->Yout[0];
}

void control() {
    Pid pid;
    for (int k = 0; k < pid.size_Yout; k++) {
        pid.rin = 1;  //阶跃信号

        pid.u = pid.kp * pid.perror + pid.ki * pid.ierror + pid.kd * pid.derror;
        //输出限幅
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
        //得到最大输出值
        if (pid.Yout[k] > pid.maxYout) {
            pid.maxYout = pid.Yout[k];
        }
        //得到最小输出值

        if (pid.Yout[k] < pid.minYout) {
            pid.minYout = pid.Yout[k];
        }
    }
}

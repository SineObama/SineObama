/*
 * MD5.h
 *
 *  Created on: 2016��10��4��
 *      Author: Sine
 */

#ifndef MD5_H_
#define MD5_H_

#include <string>

class MD5 {

 public:

    typedef long long int size;
    typedef unsigned int word;
    typedef unsigned char data_t;

    MD5();

    // һ���Լ��ܡ�lenΪλ����
    MD5 &encrypt(data_t *data, size len);

    // ÿ�δ���һ��512λ�����ݡ�����512λ��ʾ������
    void step(data_t *data, int len);

    // ��ȡ���
    std::string get();

 private:

    static const int A = 0x67452301;
    static const int B = 0xefcdab89;
    static const int C = 0x98badcfe;
    static const int D = 0x10325476;

    word state[4];
    size len;
    bool closed;

    // ����512λֱ�ӽ��м���
    void pass(data_t *);

};

#endif /* MD5_H_ */

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

    // lenΪλ����
    MD5 &encrypt(data_t *data, size len);

    // ÿ�δ���һ��512λ�����ݡ�����512λ��ʾ������
    void step(data_t *data, int len);

    // ������ֹ����ȡ���
    std::string get();

    word leftCyclicShift(word X, int s);

 private:

    static const int A = 0x67452301;
    static const int B = 0xefcdab89;
    static const int C = 0x98badcfe;
    static const int D = 0x10325476;

    word a, b, c, d;
    size len;
    bool closed;

    // ����512λֱ�ӽ��м���
    void pass(data_t *);

    void FF(word &a, word b, word c, word d, word Mj, word s, word ti);
    void GG(word &a, word b, word c, word d, word Mj, word s, word ti);
    void HH(word &a, word b, word c, word d, word Mj, word s, word ti);
    void II(word &a, word b, word c, word d, word Mj, word s, word ti);

    word F(word X, word Y, word Z);
    word G(word X, word Y, word Z);
    word H(word X, word Y, word Z);
    word I(word X, word Y, word Z);

};

#endif /* MD5_H_ */

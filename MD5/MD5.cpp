/*
 * MD5.cpp
 *
 *  Created on: 2016年10月4日
 *      Author: Sine
 */
//
#include "MD5.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

MD5::MD5() {
    a = A;
    b = B;
    c = C;
    d = D;
    len = 0;
    closed = false;
}

MD5 &MD5::encrypt(data_t *data, size len) {
    while (len > 0) {
        if (len >= 512)
            step(data, 512);
        else
            step(data, len);
        len -= 512;
        data += 512;
    }
    return *this;
}

void MD5::step(data_t *data, int len) {
    if (closed)
        throw;  // TODO
    this->len += len;
    if (len == 512) {
        pass(data);
    } else {
        /* padding */
        unsigned char s[64];
        memset(s, 0, 64);
        memcpy(s, data, (len + 7) / 8);
        int *i = (int *) s;
        s[len / 8] = (char) (1U << (7 - len % 8));
        if (len > 446) {
            pass(s);
            memset(s, 0, 64);
        }
        i[14] = (int) this->len;
        i[15] = (int) (this->len >> 32);
        pass(s);
        closed = true;
    }
}

std::string MD5::get() {
    if (!closed)
        step(NULL, 0);
    closed = true;
    std::string rtn;
    char s[100] = { };
    for (int i = 0; i < 4; i++) {
        sprintf(s, "%02x", (unsigned char) (a >> (i * 8)));
        // FIXME 出问题的语句
        rtn = rtn + s;
    }
    for (int i = 0; i < 4; i++) {
        sprintf(s, "%02x", (data_t) (b >> i * 8));
        rtn += s;
    }
    for (int i = 0; i < 4; i++) {
        sprintf(s, "%02x", (data_t) (c >> i * 8));
        rtn += s;
    }
    for (int i = 0; i < 4; i++) {
        sprintf(s, "%02x", (data_t) (d >> i * 8));
        rtn += s;
    }
    return rtn;
}

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

void MD5::pass(data_t *_s) {
    if (1) {
        word A = a;
        word B = b;
        word C = c;
        word D = d;
//#define decode
#ifdef decode
        word x[16];
        for (int i = 0, j = 0; j < 64; i++, j += 4)
        x[i] = ((word) _s[j]) | (((word) _s[j + 1]) << 8)
        | (((word) _s[j + 2]) << 16) | (((word) _s[j + 3]) << 24);
#else
        word *x = (word *) _s;
#endif
        /* Round 1 */
        FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
        FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
        FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
        FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
        FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
        FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
        FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
        FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
        FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
        FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
        FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
        FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
        FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
        FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
        FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
        FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

        /* Round 2 */
        GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
        GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
        GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
        GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
        GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
        GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
        GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
        GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
        GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
        GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
        GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
        GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
        GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
        GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
        GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
        GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

        /* Round 3 */
        HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
        HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
        HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
        HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
        HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
        HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
        HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
        HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
        HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
        HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
        HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
        HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
        HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
        HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
        HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
        HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

        /* Round 4 */
        II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
        II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
        II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
        II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
        II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
        II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
        II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
        II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
        II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
        II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
        II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
        II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
        II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
        II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
        II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
        II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

        a += A;
        b += B;
        c += C;
        d += D;
    }
}

void MD5::FF(word &a, word b, word c, word d, word Mj, word s, word ti) {
    a = b + leftCyclicShift((a + F(b, c, d) + Mj + ti), s);
}

void MD5::GG(word &a, word b, word c, word d, word Mj, word s, word ti) {
    a = b + leftCyclicShift((a + G(b, c, d) + Mj + ti), s);
}

void MD5::HH(word &a, word b, word c, word d, word Mj, word s, word ti) {
    a = b + leftCyclicShift((a + H(b, c, d) + Mj + ti), s);
}

void MD5::II(word &a, word b, word c, word d, word Mj, word s, word ti) {
    a = b + leftCyclicShift((a + I(b, c, d) + Mj + ti), s);
}

MD5::word MD5::leftCyclicShift(word X, int s) {
//    return (((((t >> (s - 1)) & X) >> 1) & 0x7FFFFFFF) >> (31 - s))
//            | (X << s);
    return (X >> (32 - s)) | (X << s);
//    while (s--)
//        X = (X & 0x8000000) ? ((X << 1) | 0x00000001) : (X << 1);
//    return X;
}

MD5::word MD5::F(word X, word Y, word Z) {
    return (X & Y) | ((~X) & Z);
}

MD5::word MD5::G(word X, word Y, word Z) {
    return (X & Z) | (Y & (~Z));
}

MD5::word MD5::H(word X, word Y, word Z) {
    return X ^ Y ^ Z;
}

MD5::word MD5::I(word X, word Y, word Z) {
    return Y ^ (X | (~Z));
}

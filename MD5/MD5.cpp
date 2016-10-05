/*
 * MD5.cpp
 *
 *  Created on: 2016Äê10ÔÂ4ÈÕ
 *      Author: Sine
 */
//
#include "MD5.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>

MD5::MD5() {
    state[0] = A;
    state[1] = B;
    state[2] = C;
    state[3] = D;
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
        data_t s[64];
        memset(s, 0, 64);
        memcpy(s, data, (len + 7) / 8);
        word *i = (word *) s;
        s[len / 8] = (data_t) (1U << (7 - len % 8));
        if (len > 446) {
            pass(s);
            memset(s, 0, 64);
        }
        i[14] = (word) this->len;
        i[15] = (word) (this->len >> 32);
        pass(s);
        closed = true;
    }
}

std::string MD5::get() {
    if (!closed)
        step(NULL, 0);
    closed = true;
    std::string rtn;
    unsigned char *ss = (unsigned char *) state;
    char s[10] = { };
    for (int i = 0; i < 16; i++) {
        sprintf(s, "%02x", ss[i]);
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

#define F_func(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G_func(X, Y, Z) (((X) & (Z)) | ((Y) & (~(Z))))
#define H_func(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I_func(X, Y, Z) ((Y) ^ ((X) | (~(Z))))

#define leftCyclicShift(X, s) (((X) >> (32 - (s))) | ((X) << (s)))

#define invoke(name, a, b, c, d, x, s, ac) {\
    a += name##_func(b, c, d) + (x) + (ac);\
    a = (b) + leftCyclicShift(a, s); }

void MD5::pass(data_t *_s) {
    word a = state[0];
    word b = state[1];
    word c = state[2];
    word d = state[3];
    word *x = (word *) _s;

    /* Round 1 */
    invoke(F, a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
    invoke(F, d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
    invoke(F, c, d, a, b, x[2], S13, 0x242070db); /* 3 */
    invoke(F, b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
    invoke(F, a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
    invoke(F, d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
    invoke(F, c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
    invoke(F, b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
    invoke(F, a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
    invoke(F, d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
    invoke(F, c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
    invoke(F, b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
    invoke(F, a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
    invoke(F, d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
    invoke(F, c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
    invoke(F, b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

    /* Round 2 */
    invoke(G, a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
    invoke(G, d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
    invoke(G, c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
    invoke(G, b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
    invoke(G, a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
    invoke(G, d, a, b, c, x[10], S22, 0x2441453); /* 22 */
    invoke(G, c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
    invoke(G, b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
    invoke(G, a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
    invoke(G, d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
    invoke(G, c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
    invoke(G, b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
    invoke(G, a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
    invoke(G, d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
    invoke(G, c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
    invoke(G, b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    invoke(H, a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
    invoke(H, d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
    invoke(H, c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
    invoke(H, b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
    invoke(H, a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
    invoke(H, d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
    invoke(H, c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
    invoke(H, b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
    invoke(H, a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
    invoke(H, d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
    invoke(H, c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
    invoke(H, b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
    invoke(H, a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
    invoke(H, d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
    invoke(H, c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
    invoke(H, b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    invoke(I, a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
    invoke(I, d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
    invoke(I, c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
    invoke(I, b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
    invoke(I, a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
    invoke(I, d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
    invoke(I, c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
    invoke(I, b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
    invoke(I, a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
    invoke(I, d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
    invoke(I, c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
    invoke(I, b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
    invoke(I, a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
    invoke(I, d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
    invoke(I, c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
    invoke(I, b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

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
    output_t *ss = (output_t *) state;
    char s[10] = { };
    for (int i = 0; i < 16; i++) {
        sprintf(s, "%02x", ss[i]);
        rtn = rtn + s;
    }
    return rtn;
}

#define F_func(X, Y, Z) (((X) & (Y)) | ((~(X)) & (Z)))
#define G_func(X, Y, Z) (((X) & (Z)) | ((Y) & (~(Z))))
#define H_func(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I_func(X, Y, Z) ((Y) ^ ((X) | (~(Z))))

#define leftCyclicShift(X, s) (((X) >> (32 - (s))) | ((X) << (s)))

#define invoke(name, a, b, c, d, x, s, ac) {\
    a += name##_func(b, c, d) + (x) + (ac);\
    a = (b) + leftCyclicShift(a, s); }

void MD5::pass(data_t *_s) {
    static int s[4][4] = { { 7, 12, 17, 22 },
                           { 5, 9, 14, 20 },
                           { 4, 11, 16, 23 },
                           { 6, 10, 15, 21 } };
    static unsigned int ac[4][16] = { { 0xd76aa478,
                                        0xe8c7b756,
                                        0x242070db,
                                        0xc1bdceee,
                                        0xf57c0faf,
                                        0x4787c62a,
                                        0xa8304613,
                                        0xfd469501,
                                        0x698098d8,
                                        0x8b44f7af,
                                        0xffff5bb1,
                                        0x895cd7be,
                                        0x6b901122,
                                        0xfd987193,
                                        0xa679438e,
                                        0x49b40821 },
                                      { 0xf61e2562,
                                        0xc040b340,
                                        0x265e5a51,
                                        0xe9b6c7aa,
                                        0xd62f105d,
                                        0x2441453,
                                        0xd8a1e681,
                                        0xe7d3fbc8,
                                        0x21e1cde6,
                                        0xc33707d6,
                                        0xf4d50d87,
                                        0x455a14ed,
                                        0xa9e3e905,
                                        0xfcefa3f8,
                                        0x676f02d9,
                                        0x8d2a4c8a },
                                      { 0xfffa3942,
                                        0x8771f681,
                                        0x6d9d6122,
                                        0xfde5380c,
                                        0xa4beea44,
                                        0x4bdecfa9,
                                        0xf6bb4b60,
                                        0xbebfbc70,
                                        0x289b7ec6,
                                        0xeaa127fa,
                                        0xd4ef3085,
                                        0x4881d05,
                                        0xd9d4d039,
                                        0xe6db99e5,
                                        0x1fa27cf8,
                                        0xc4ac5665 },
                                      { 0xf4292244,
                                        0x432aff97,
                                        0xab9423a7,
                                        0xfc93a039,
                                        0x655b59c3,
                                        0x8f0ccc92,
                                        0xffeff47d,
                                        0x85845dd1,
                                        0x6fa87e4f,
                                        0xfe2ce6e0,
                                        0xa3014314,
                                        0x4e0811a1,
                                        0xf7537e82,
                                        0xbd3af235,
                                        0x2ad7d2bb,
                                        0xeb86d391 } };

    word A = state[0];
    word B = state[1];
    word C = state[2];
    word D = state[3];
    word *x = (word *) _s;
    for (int i = 16, j = 0; i > 0; i--, j++)
        invoke(F, state[i % 4], state[(i + 1) % 4], state[(i + 2) % 4],
               state[(i + 3) % 4], x[j], s[0][j % 4], ac[0][j]);
    for (int i = 16, j = 0; i > 0; i--, j++)
        invoke(G, state[i % 4], state[(i + 1) % 4], state[(i + 2) % 4],
               state[(i + 3) % 4], x[(1 + 5 * j) % 16], s[1][j % 4], ac[1][j]);
    for (int i = 16, j = 0; i > 0; i--, j++)
        invoke(H, state[i % 4], state[(i + 1) % 4], state[(i + 2) % 4],
               state[(i + 3) % 4], x[(5 + 3 * j) % 16], s[2][j % 4], ac[2][j]);
    for (int i = 16, j = 0; i > 0; i--, j++)
        invoke(I, state[i % 4], state[(i + 1) % 4], state[(i + 2) % 4],
               state[(i + 3) % 4], x[(7 * j) % 16], s[3][j % 4], ac[3][j]);
    state[0] += A;
    state[1] += B;
    state[2] += C;
    state[3] += D;
}

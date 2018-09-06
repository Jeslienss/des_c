//
// Created by jiawei on 9/2/18.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "des.h"


const uint8_t pc1[] = {57, 49, 41, 33, 25, 17, 9,
                       1, 58, 50, 42, 34, 26, 18,
                       10, 2, 59, 51, 43, 35, 27,
                       19, 11, 3, 60, 52, 44, 36,
                       63, 55, 47, 39, 31, 23, 15,
                       7, 62, 54, 46, 38, 30, 22,
                       14, 6, 61, 53, 45, 37, 29,
                       21, 13, 5, 28, 20, 12, 4};


const uint8_t shift_table[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};


const uint8_t pc2[] = {14, 17, 11, 24, 1, 5,
                       3, 28, 15, 6, 21, 10,
                       23, 19, 12, 4, 26, 8,
                       16, 7, 27, 20, 13, 2,
                       41, 52, 31, 37, 47, 55,
                       30, 40, 51, 45, 33, 48,
                       44, 49, 39, 56, 34, 53,
                       46, 42, 50, 36, 29, 32,};

const uint8_t ip_table[] = {58, 50, 42, 34, 26, 18, 10, 2,
                            60, 52, 44, 36, 28, 20, 12, 4,
                            62, 54, 46, 38, 30, 22, 14, 6,
                            64, 56, 48, 40, 32, 24, 16, 8,
                            57, 49, 41, 33, 25, 17, 9, 1,
                            59, 51, 43, 35, 27, 19, 11, 3,
                            61, 53, 45, 37, 29, 21, 13, 5,
                            63, 55, 47, 39, 31, 23, 15, 7,};

const uint8_t sbox[8][4][16] = {
        {{14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7},
                {0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8},
                {4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0},
                {15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13},},

        {{15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10},
                {3,  13, 4,  7,  15, 2,  8,  14, 12, 0, 1,  10, 6,  9,  11, 5},
                {0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15},
                {13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5, 14, 9},},

        {{10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8},
                {13, 7,  0,  9,  3,  4,  6,  10, 2,  8, 5,  14, 12, 11, 15, 1},
                {13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7},
                {1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5, 2,  12},},

        {{7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15},
                {13, 8,  11, 5,  6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9},
                {10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4},
                {3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14},},

        {{2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9},
                {14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6},
                {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14},
                {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3},},

        {{12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11},
                {10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8},
                {9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6},
                {4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13},},

        {{4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1},
                {13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6},
                {1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2},
                {6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12},},

        {{13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7},
                {1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2},
                {7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8},
                {2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11},}};


const uint8_t ebox[] = {32, 1, 2, 3, 4, 5, 4, 5,
                        6, 7, 8, 9, 8, 9, 10, 11,
                        12, 13, 12, 13, 14, 15, 16, 17,
                        16, 17, 18, 19, 20, 21, 20, 21,
                        22, 23, 24, 25, 24, 25, 26, 27,
                        28, 29, 28, 29, 30, 31, 32, 1};


const uint8_t ptable[] = {16, 7, 20, 21,
                          29, 12, 28, 17,
                          1, 15, 23, 26,
                          5, 18, 31, 10,
                          2, 8, 24, 14,
                          32, 27, 3, 9,
                          19, 13, 30, 6,
                          22, 11, 4, 25};


const uint8_t iptable_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
                             39, 7, 47, 15, 55, 23, 63, 31,
                             38, 6, 46, 14, 54, 22, 62, 30,
                             37, 5, 45, 13, 53, 21, 61, 29,
                             36, 4, 44, 12, 52, 20, 60, 28,
                             35, 3, 43, 11, 51, 19, 59, 27,
                             34, 2, 42, 10, 50, 18, 58, 26,
                             33, 1, 41, 9, 49, 17, 57, 25};

void convertStr2Hex(char *key, uint64_t *binKey) {
    *binKey = 0;
    char hashStr2Num[] = "0123456789ABCDEF";
    uint64_t i, j;
    for (i = 0; i < strlen(key); i++) {
        for (j = 0; j < strlen(hashStr2Num); j++) {
            if (*(key + i) == hashStr2Num[j]) {
                *binKey <<= 4;
                *binKey |= j;
                break;
            }
        }
    }
}


uint64_t ariShift(uint64_t f, uint8_t t) {
    if (t == 1)
        return ((f & 0x8000000) >> 27) | ((f << 1) & 0xfffffff);
    else if (t == 2)
        return ((f & 0xc000000) >> 26) | ((f << 2) & 0xfffffff);
}


void genSubKey(uint64_t *binKey, uint64_t *subKey) {
    uint64_t *pk = malloc(sizeof(uint64_t));
    uint64_t *cd = malloc(sizeof(uint64_t) * (2 * 16 + 2));
    //uint64_t *subKey = malloc(sizeof(uint64_t) * 16);
    *pk = 0;
    uint64_t i, j, temp;

    int pc1_len = sizeof(pc1) / sizeof(uint8_t);
    int pc2_len = sizeof(pc2) / sizeof(uint8_t);

    for (i = 0; i < pc1_len; i++) {
        *pk <<= 1;
        *pk |= (*binKey >> (64 - pc1[i])) & 0x01;
    }

    *(cd + 0) = (*pk >> 28) & 0xfffffff; // left 28 bits, c0
    *(cd + 1) = *pk & 0xfffffff; // right 28 bits, d0

    for (i = 1; i <= 16; i++) {
        *(cd + 2 * i + 0) = ariShift(*(cd + 2 * (i - 1) + 0), shift_table[i - 1]);// c_n
        *(cd + 2 * i + 1) = ariShift(*(cd + 2 * (i - 1) + 1), shift_table[i - 1]);// d_n
    }

    for (i = 1; i <= 16; i++) {
        temp = (*(cd + 2 * i) << 28) | *(cd + 2 * i + 1);
        for (j = 0; j < pc2_len; j++) {
            *(subKey + i - 1) <<= 1;
            *(subKey + i - 1) |= (temp >> (56 - pc2[j])) & 0x01;
        }
    }

    free(pk);
    free(cd);
}


void encodeData(uint64_t *subKey, uint64_t *m, uint64_t *c) {
    uint64_t *ip = malloc(sizeof(uint64_t));
    uint64_t ln, rn, ln_1, rn_1, Ern_1, temp, local6bits, f, rl, ip_1;
    int i, j;
    int len_iptable = sizeof(ip_table) / sizeof(uint8_t);
    for (i = 0; i < len_iptable; i++) {
        *ip <<= 1;
        *ip |= (*m >> (64 - ip_table[i])) & 0x01;
    }

    ln_1 = (*ip >> 32) & 0xffffffff;
    rn_1 = *ip & 0xffffffff;

    for (i = 0; i < 16; i++) {
        Ern_1 = 0;
        for (j = 0; j < 48; j++) {
            Ern_1 <<= 1;
            Ern_1 |= (rn_1 >> (32 - ebox[j])) & 0x01;
        }
        Ern_1 ^= *(subKey + i);

        // sbox
        temp = 0;
        for (j = 0; j < 8; j++) {
            local6bits = (Ern_1 >> (7 - j) * 6) & 0x3f;
            temp <<= 4;
            temp |= sbox[j][((local6bits >> 4) & 0x2) | (local6bits & 0x1)][(local6bits & 0x1e) >> 1];
        }
        // ptable
        f = 0;
        for (j = 0; j < 32; j++) {
            f <<= 1;
            f |= (temp >> (32 - ptable[j])) & 0x01;
        }

        ln = rn_1;
        rn = ln_1 ^ f;
        ln_1 = ln;
        rn_1 = rn;
    }
    rl = ((rn & 0xffffffff) << 32) | (ln & 0xffffffff);

    for (i = 0; i < 64; i++) {
        *c <<= 1;
        *c |= (rl >> (64 - iptable_1[i])) & 0x01;
    }
}


void des(char *key) {
    uint64_t *binKey = malloc(sizeof(uint64_t));
    uint64_t *subKey = malloc(sizeof(uint64_t) * 16);
    convertStr2Hex(key, binKey);

    //printf("%jx\n", *binKey); // using this format to print uint64_t number
    genSubKey(binKey, subKey);

    uint64_t *m = malloc(sizeof(uint64_t));
    uint64_t *c = malloc(sizeof(uint64_t));
    *m = 0x0123456789ABCDEF;

    encodeData(subKey, m, c);

    free(binKey);
    free(subKey);
    free(m);
    free(c);
}

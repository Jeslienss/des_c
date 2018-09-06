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

const uint8_t ip[] = {58, 50, 42, 34, 26, 18, 10, 2,
                      60, 52, 44, 36, 28, 20, 12, 4,
                      62, 54, 46, 38, 30, 22, 14, 6,
                      64, 56, 48, 40, 32, 24, 16, 8,
                      57, 49, 41, 33, 25, 17, 9, 1,
                      59, 51, 43, 35, 27, 19, 11, 3,
                      61, 53, 45, 37, 29, 21, 13, 5,
                      63, 55, 47, 39, 31, 23, 15, 7,};


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


void genPermutedKey(uint64_t *binKey, uint64_t *subKey) {
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


void des(char *key) {
    uint64_t *binKey = malloc(sizeof(uint64_t));
    uint64_t *subKey = malloc(sizeof(uint64_t) * 16);
    convertStr2Hex(key, binKey);

    //printf("%jx\n", *binKey); // using this format to print uint64_t number
    genPermutedKey(binKey, subKey);

    free(binKey);
    free(subKey);
}

//
// Created by jiawei on 9/2/18.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "des.h"


void convertStr2Hex(char* key, uint8_t* binKey) {
    char hashStr2Num[] = "0123456789ABCDEF";
    uint8_t i, j;
    for (i = 0 ; i < strlen(key) / 2; i++) {
        uint8_t front = 0, back = 0;
        for (j = 0; j < strlen(hashStr2Num); j++) {
            if (*(key + i * 2) == hashStr2Num[j]) {
                front = j;
                break;
            }
        }
        for (j = 0; j < strlen(hashStr2Num); j++) {
            if (*(key + i * 2 + 1) == hashStr2Num[j]) {
                back = j;
                break;
            }
        }
        *(binKey + i) = (front << 4) + back;
    }
}


void genPermutedKey(uint8_t* binKey) {
    uint8_t pc1[] = {57,   49,    41,   33,    25,    17,    9,
                     1,    58,    50,   42,    34,    26,   18,
                     10,    2,    59,   51,    43,    35,   27,
                     19,   11,     3,   60,    52,    44,   36,
                     63,   55,    47,   39,    31,    23,   15,
                     7,   62,    54,   46,    38,    30,   22,
                     14,    6,    61,   53,    45,    37,   29,
                     21,   13,     5,   28,   20,    12,    4};
    uint8_t* pk = malloc(sizeof(uint8_t) * 7);
    uint8_t* c0 = malloc()
    uint8_t i, j, _index, temp;

    for (i = 0; i < 7; i++) {
        temp = 0;
        for (j = 0; j< 8; j++) {
            _index = pc1[i * 8 + j];
            temp |= ((*(binKey + (_index - 1) / 8) >> ((_index % 8 == 0) ? 0:(8 - _index % 8))) & 1) << (7 - j);
        }
        *(pk + i) = temp;
    }

}


void des(char* key) {
    uint8_t* binKey = malloc(KEYLEN * sizeof(uint8_t) / 2);
    convertStr2Hex(key, binKey);
    genPermutedKey(binKey);
    int i;


    //uint8_t _index;
    //for (_index = 1; _index <= 64; _index ++)
    //    printf("%d", (*(binKey + (_index - 1) / 8) >> ((_index % 8 == 0) ? 0:(8 - _index % 8))) & 1);
}

//1111000 0110011 0010101 0101111 0101010 1011001 1001111 0001111
//1111000 0110011 0010101 0101111 0101010 1011001 1001111 0001111
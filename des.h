//
// Created by jiawei on 9/2/18.
//

#ifndef DES_DES_H
#define DES_DES_H

#define KEYLEN 16

#include <stdint.h>

void convertStr2Hex(char* key, uint64_t* binKey);
void genPermutedKey(uint64_t* binKey, uint64_t *subKey);
void des(char* key);
uint64_t ariShift(uint64_t f, uint8_t t);


#endif //DES_DES_H

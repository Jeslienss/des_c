//
// Created by jiawei on 9/2/18.
//

#ifndef DES_DES_H
#define DES_DES_H

#define KEYLEN 16

#include <stdint.h>

void convertStr2Hex(char* key, uint64_t* binKey);
void genSubKey(uint64_t* binKey, uint64_t *subKey);
void des(char* key);
void proBlock(uint64_t *subKey, uint64_t *m, uint64_t *c, int flag);
uint64_t ariShift(uint64_t f, uint8_t t);



#endif //DES_DES_H

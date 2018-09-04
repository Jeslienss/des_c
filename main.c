#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "des.h"


int main() {
    char key[] = "133457799BBCDFF1";

    des(key);

    return 0;
}

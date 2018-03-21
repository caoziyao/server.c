//
//  main.c
// cc *.c core/*.c -Ilua-5.3.4/src  -llua -Llua-5.3.4/src -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib && ./a.out
// test:  siege -c 10 -r 1000 http://127.0.0.1:3000/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sds.h"

int
main(int argc, const char *argv[]) {
    
    printf("cache\n");
    
    return 0;
}

//
//  main.c
//  make
//  todo cache redis
// 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "test.h"
#include "redis.h"

int
main(int argc, const char *argv[]) {
    
    printf("cache\n");
    
    redisServerTest();
    return 0;
}

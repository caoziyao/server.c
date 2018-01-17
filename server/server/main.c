//
//  main.c
//  web server
//
//  Created by working on 2018/1/17.
//  Copyright © 2018年 working. All rights reserved.
//

/*
 test:  curl -v localhost:3000
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include "socket_server.h"
#include "python_embed.h"

#define MaxBuffer 1024

void *
response(void *socketFile) {
    int s = *(int *)socketFile;
    
    char r[MaxBuffer];
    read(s, r, sizeof(r));
    
    char *message = callPythonFun(r);
    write(s , message , strlen(message));
    printf("s: %u %s\n", (unsigned int)s, message);
    
    printf("r: %s", r);
    close(s);
    return NULL;
}

void
init() {
    initPythonEnv();
}

int main(int argc, const char * argv[]) {
    unsigned int port = 3000;
    int s = openSocket(port);
    
    init();
    while (true) {
        int *n = malloc(sizeof(int));
        *n = acceptClient(s);
        
        printf("accept a socket\n");
        
        pthread_t t;
        pthread_create(&t, NULL, response, (void *)n);

        
    }
    return 0;
}

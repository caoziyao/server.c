//
//  main.c
//  web server
//
//  Created by working on 2018/1/11.
//  Copyright © 2018年 working. All rights reserved.
//
/*
 cc  -I/Users/working/.pyenv/versions/3.6-dev/include/python3.6m -L/Users/working/.pyenv/versions/3.6-dev/lib/python3.6/config-3.6m-darwin -lpython3.6m *.c -o main.exe && ./main.exe
 */
/*
 test:  curl -v localhost:3000
 想明白思路
 
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
    int e = read(s, r, sizeof(r));
    
    if (e != -1 ){
        printf("e: %d\n", e);
        char *message = callPythonFun(r);
        write(s , message , strlen(message));
        //    printf("msg %s\n", (unsigned int)s, message);
        printf("r: %s", r);
    }
    
    close(s);
    return NULL;
}


void
init() {
    initPythonEnv();
}


void *
listenConnect(void *socketFile) {
    int s = *(int *)socketFile;
    
    struct sockaddr_in client;
    int size = sizeof(struct sockaddr_in);
    
    while (true) {
        int *n = malloc(sizeof(int));
        *n = acceptClient(s);
//      int clientSocket = accept(s, (struct sockaddr *)&client, (socklen_t*)&size);
        
        pthread_t t;
        pthread_create(&t, NULL, response, (void *)n);
    }
    
    return NULL;
}


void
pThreadServer() {
    unsigned int port = 3000;
    int s = openSocket(port);
    
    init();
    
    pthread_t tid[20];
    for(int i = 0; i < 20; i++) {
        pthread_create(&tid[i], NULL, listenConnect, &s);
    }
    
    for(int i = 0; i < 20; i++) {
        pthread_join(tid[i], NULL);
    }

}


int main(int argc, const char * argv[]) {
    pThreadServer();
    return 0;
}

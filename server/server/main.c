//
//  main.c
//
//  Created by working on 2018/1/19.
//  Copyright © 2018年 working. All rights reserved.
// cc *.c -Ilua-5.3.4/src -llua -Llua-5.3.4/src && ./a.out


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "threadpool.h"
#include "response.h"
#include "gwsocket.h"
#include "gwkqueue.h"

int
main(int argc, const char *argv[]) {

    // kqueue
    int epollfd = initKqueue();
    
    
    // socket
    unsigned int port = 3000;
    int s = openSocket(port);
    setNonBlock(s);

    struct sockaddr_in client;
    int size = sizeof(struct sockaddr_in);
    
    // 注册事件
    updateEvents(epollfd, s);

    while (true) {
        loopOnce(epollfd, s, 10000);
//        int *n = malloc(sizeof(int));
//        *n = accept(s, (struct sockaddr *)&client, (socklen_t *)&size);
//
//        printf("(add task %d)\n", *n);
    }

    

    return 0;
}

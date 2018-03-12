//
//  gwsocket.c
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

#include "gwsocket.h"

void
setNonBlock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        quit("fcntl()");
    }
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    
    if (r < 0) {
        quit("fcntl()");
    }
}


int
openSocket(unsigned short port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        quit("socket()");
    }
    
    // 消除端口占用错误
    const int option = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *)&option , sizeof(int));
    
    //
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    //
    bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(s, 5);
    //
    printf("listening at port %d\n", port);
    return s;
    
}

//
//  socket_server.c
//  server
//
//  Created by working on 2018/1/17.
//  Copyright © 2018年 working. All rights reserved.
//

#include "socket_server.h"


int
openSocket(unsigned short port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    
    //
    const int option = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *)&option , sizeof(int));
    
    //
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    //
    //
    bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(s, 5);
    //
    printf("listening at port %d\n", port);
    return s;
    
}

int
acceptClient(int socketFile) {
    
    int s = socketFile;
    struct sockaddr_in client;
    int size = sizeof(struct sockaddr_in);
    
    int clientSocket = accept(s, (struct sockaddr *)&client, (socklen_t *)&size);
    
    return clientSocket;

}

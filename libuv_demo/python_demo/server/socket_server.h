//
//  socket_server.h
//  server
//
//  Created by working on 2018/1/17.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef socket_server_h
#define socket_server_h

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int
openSocket(unsigned short port);

int
acceptClient(int socketFile);

#endif /* socket_server_h */

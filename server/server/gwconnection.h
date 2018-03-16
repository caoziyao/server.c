//
//  gwconnection.h
//  server


#ifndef gwconnection_h
#define gwconnection_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>


// 绑定 socket
int
GwConnOpenSocket(unsigned short port);

// 设置非阻塞
void
GwConnSetNonBlock(int fd);

void
GwConnCloseLuaEnv();

void *
response(void *socketFile);

#endif /* gwconnection_h */

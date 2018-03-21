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
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "utils.h"
#include "gwlua.h"
#include "config.h"

typedef enum _EnumProtocol EnumProtocol;
enum _EnumProtocol{
    EnumProtocolHttp = 0,
    EnumProtocolHttps = 1
};

typedef struct _GwConnection GwConnection;
struct _GwConnection{
    unsigned short port;
    int server;
    SSL_CTX *ctx;
    EnumProtocol protocol;
};

// create server socket
GwConnection *
GwConnOpenSocket(unsigned short port);

// 设置非阻塞
void
GwConnSetNonBlock(int fd);

void
GwConnCloseLuaEnv();

void *
GwConnResponse(void *socketFile);

GwConnection *
GwConnSSLOpenSocket(unsigned short port);

/* release context */
void
GwConnSSLFree(GwConnection *conn);

/* Serve the connection -- threadable */
void
Servlet(SSL* ssl);

// test ssl
int
GwConnSSLTest();

#endif /* gwconnection_h */

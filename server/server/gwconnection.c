//
//  gwconnection.c
//  server
//

#include "gwconnection.h"
#include "utils.h"
#include "gwlua.h"
#include "config.h"

typedef struct _GwConnection GwConnection;
struct _GwConnection{
    
};


int
GwConnOpenSocket(unsigned short port) {
    
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
    
    // bind，成功返回0，出错返回-1
    bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(s, 5);
    //
    printf("listening at port %d\n", port);
    
    return s;
}


void
GwConnSetNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        quit("fcntl()1");
    }
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    
    if (r < 0) {
        quit("fcntl()2");
    }
}


void *
GwConnResponse(void *socketFile){
    int s = *(int *)socketFile;
    
    char r[MaxRecvBuff];
    read(s, r, sizeof(r));
    
    // add 函数中调用了一个 lua 中的函数
    const char *message = GuwLuaExecl("server_lua/server.lua", r);
    
    write(s , message , strlen(message));
    //    printf("s: %u %s\n", (unsigned int)s, message);
    close(s);
    return NULL;
}




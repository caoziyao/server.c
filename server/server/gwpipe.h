//
//  gwpipe.h
//  server
//
//  Created by working on 2018/3/14.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwpipe_h
#define gwpipe_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "utils.h"

// 定义文件描述符
typedef enum _GwPipeFdEnd {
    EGwPipeRead = 0,
    EGwPipeWrite = 1
}GwPipeFdEnd;

typedef struct _GwPipeData {
    int s;
    char *msg;
}GwPipeData;

// 初始化 pipe，返回文件描述符 pipefd
// pipefd[0]指向管道的读端, pipefd[1]指向管道的写端，
int *
GwPipeInit();

// 关闭 pipe 一端
void
GwPipeClose(int *fd, GwPipeFdEnd end);

// 读取 pipe
// todo 优化
void
GwPipeRead(int *fd, char *msg, int n) ;

// 写入 pipe
void
GwPipeWrite(int *fd, char *msg, int n);

// todo struct
void
GwPipeReadData(int *fd, void *s);

// todo struct
void
GwPipeWriteData(int *fd, void *s);

#endif /* gwpipe_h */

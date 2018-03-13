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

// 定义文件描述符
typedef enum _GwPipeFdEnd{
    EGwPipeRead = 0,
    EGwPipeWrite = 1
}GwPipeFdEnd;


// 初始化 pipe，返回文件描述符 pipefd
// pipefd[0]指向管道的读端, pipefd[1]指向管道的写端，
int *
GwPipeInit();


// 关闭 pipe 一端
void
GwPipeClose(int *fd, GwPipeFdEnd end);


// 读取 pipe
// todo 优化
char *
GwPipeRead(int *fd);


// 写入 pipe
void
GwPipeWrite(int *fd, char *msg, int n);


#endif /* gwpipe_h */

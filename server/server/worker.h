//
//  worker.h
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

/*
 worker进程来处理基本的网络事件
 1, 一个请求，只可能在一个worker进程中处理
 2, 一个worker进程，不可能处理其它进程的请求
 3, worker进程的个数是可以设置的
 
 */

#ifndef worker_h
#define worker_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
GwWorkerRun(int fd, int socketFile);

#endif /* worker_h */

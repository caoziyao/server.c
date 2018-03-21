//
//  worker.h
//  server


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

#include "threadpool.h"
#include "gwkqueue.h"
#include "gwpipe.h"
#include "config.h"
#include "gwtimer.h"
#include "gwshm.h"
#include "gwmutex.h"

#include "gwconnection.h"
#include "gwpool.h"

// 定义 worker
typedef struct _GwWorker GwWorker;
struct _GwWorker{
    GwPool *pool;
};

void
GwWorkerRun(int shmid, GwConnection *conn);

#endif /* worker_h */

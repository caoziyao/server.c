//
//  master.h
//  server


/*
 master进程管理worker进程
 1, 接收来自外界的信号
 2, 向各worker进程发送信号
 3, 监控worker进程的运行状态
 4, 当worker进程退出后(异常情况下)，会自动重新启动新的worker进程
 
 */

#ifndef master_h
#define master_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "gwshm.h"
#include "worker.h"

typedef struct _GwMaster GwMaster;

// 定义 worker
typedef void (*GwWorkerAction)(GwMaster *master, int socketFile);

struct _GwMaster{
    int workerId[NumberOfWorker];
    int ret;
    GwShm *shm;
    GwWorkerAction action;
};

// 初始化 master
GwMaster *
GwMasterInit();

// 启动worker进程
void
GwMasterStartWorker(GwMaster *master, int socketFile);

#endif /* master_h */

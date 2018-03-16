//
//  gwmonitor.c
//  server
//
//  Created by working on 2018/3/15.
//  Copyright © 2018年 working. All rights reserved.
//

/*
 monitor
 子进程收到终止信号后，完成自身善后工作，然后直接调用exit()终止。
 父进程收到子进程终止的信号后，先调用waitpid()回收子进程的资源，避免僵尸进程，再拉起新的子进程。
 父进程收到终止信号后，首先调用kill()通知子进程终止，然后调用waitpid()等待子进程终止并回收子进程的资源，最后父进程调用exit()终止。
 */

#include "gwmonitor.h"


GwMonitor *
GwMonitorInit() {
    return NULL;
}


void
GwMonitorAddPid(GwMonitor *monitor, int pid){
    monitor->n++;
    int n = monitor->n;
    monitor->chlidId[n] = pid;
}


void
GwMonitorRun(GwMonitor *monitor) {
    int wpid, status;
    while ((wpid = wait(&status)) > 0) {
        int i = WEXITSTATUS(status); // 进程的返回值
        int wif = WIFEXITED(status); // 子进程是否为正常退出的，如果是，它会返回一个非零值
        printf("status: %d %d %d\n", i, wif, wpid);
    }
     printf("ssstatus: %d %d\n", status, wpid);
}

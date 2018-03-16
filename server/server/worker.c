//
//  worker.c
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

/*
 惊群：
 利用一把进程间锁，每个进程中都尝试获得这把锁，
 如果获取成功将监听socket加入wait集合中，并设置超时等待连接到来，
 没有获得所的进程则将监听socket从wait集合去除
 */

#include "worker.h"
#include "threadpool.h"
#include "gwkqueue.h"
#include "gwpipe.h"
#include "config.h"
#include "gwconnection.h"

void
GwWorkerRun(int socketFile) {
    int fd = initKqueue();
    int s = socketFile;
    
    GuaThreadPool *pool = GuaThreadPoolNew(2);
    //  GuaThreadPoolAddTask(pool, response, n);
    //
    // 注册事件，让每个进程轮流监听 socket
    //1、获得锁则加入wait集合
    //2、没有获得则去除
    GwKqueueRegister(fd, s);
    struct kevent events[MaxEventCount];
    
    while (true) {
        int ret = kevent(fd, NULL, 0, events, MaxEventCount, NULL);
        printf("ret %d\n", ret);
        GwKqueueHandleEvent(fd, events, ret, s);
    }
    
}

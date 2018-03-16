//
//  worker.c
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

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
    // 注册
    GwKqueueRegister(fd, s);
    struct kevent events[5000];
    
    while (true) {
        int ret = kevent(fd, NULL, 0, events, 5000, NULL);
        printf("ret %d\n", ret);
        GwKqueueHandleEvent(fd, events, ret, s);
    }
    
}

//
//  worker.c
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

#include "worker.h"
#include "threadpool.h"
#include "gwsocket.h"
#include "gwkqueue.h"
#include "gwpipe.h"
#include "config.h"

void
GwWorkerRun(int fd, int socketFile) {
    int s = socketFile;
//    printf("chlid %d\n", getpid());
    GuaThreadPool *pool = GuaThreadPoolNew(2);
    //        GuaThreadPoolAddTask(pool, response, n);
    while (true) {
        loopOnce(fd, s, 10000, pool);
    }
    
}

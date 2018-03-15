//
//  main.c
// cc *.c -Ilua-5.3.4/src -llua -Llua-5.3.4/src && ./a.out
// test:  siege -c 10 -r 1000 http://127.0.0.1:3000/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "threadpool.h"
#include "gwsocket.h"
#include "gwkqueue.h"
#include "config.h"
#include "master.h"
#include "worker.h"
#include "gwpipe.h"
#include "gwshm.h"
#include "gwmonitor.h"

int
main(int argc, const char *argv[]) {

//    GwShm *shm = GwShmInit();
    
    unsigned int port = 3000;
    int s = openSocket(port);
    setNonBlock(s);

    // 新进程
    pid_t id;
    GwMonitorData *monitorData = malloc(sizeof(GwMonitorData));
    monitorData->n = -1;
    int n = NumberOfWorker;
    
    for (int i = 0; i < n; i++) {
        id = fork();
        if (id == 0 || id == -1) {
            break;
        }
        GwMonitorAddPid(monitorData, id);
        printf("ffid %d\n", id);
    }
    
//    GwMasterFork(&id);
    // kqueue
    int epollfd = initKqueue();
    
    if (id < 0) {
        quit("fork()");
    } else if (id == 0) {
        // child
        int childid = getpid();
        
        int lr = listen(s, 5);
        printf("lr %d\n", lr);
        printf("chlid %d %d %p\n", getpid(), s, &s);
        // 注册事件
        
        initLuaEnv();
        updateEvents(epollfd, s, GwKQueueFilterRead | GwKQueueFilterWrite, GwKQueueFlagAdd);
        // run worker
//        GwWorkerRun(epollfd, s);
//        while (1) {
//            ;
//        }
        sleep(1);
        printf("1\n");
        exit(2);
        
    } else {
        // parent
        sleep(15);
        printf("2\n");
        GwMonitorRun(monitorData);

    }

    return 0;
}

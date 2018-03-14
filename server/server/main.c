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

int
main(int argc, const char *argv[]) {
    
    GwShm *shm = GwShmInit();
    
    pid_t id;
    GwMasterFork(&id);
    
    int epollfd = initKqueue();
    
    if (id < 0) {
        quit("fork()");
    } else if (id == 0) {
        // child
        sleep(1);
        
        GwShmData *data = GwShmReadData(shm);
        char *msg = data->msg;
//        int *s = data->s;
        unsigned int port = 3000;
        int s = openSocket(port);
        setNonBlock(s);
        
        printf("chlid %d %d %p %s\n", getpid(), s, &s, msg);
        // 注册事件
        initLuaEnv();
        updateEvents(epollfd, s, GwKQueueFilterRead | GwKQueueFilterWrite, GwKQueueFlagAdd);
        // run worker
        GwWorkerRun(epollfd, s);
        
    } else {
        // parent
        GwShmData *data = malloc(sizeof(GwShmData));
        data->msg = "abcd hello";
        data->s = 12;
    
        printf("parents %d %d %p\n", getpid(), data->s, &data->s);
        GwShmWriteData(shm, data);
        wait(NULL);
    }
 

    return 0;
}

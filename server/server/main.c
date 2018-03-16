//
//  main.c
// cc *.c -Ilua-5.3.4/src -llua -Llua-5.3.4/src && ./a.out
// test:  siege -c 10 -r 1000 http://127.0.0.1:3000/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "threadpool.h"
#include "gwkqueue.h"
#include "config.h"
#include "master.h"
#include "worker.h"
#include "gwpipe.h"
#include "gwshm.h"
#include "gwmonitor.h"

#include "gwconnection.h"
#include "gwlua.h"


int
main(int argc, const char *argv[]) {

    // socket
    unsigned int port = 3000;
    int s = GwConnOpenSocket(port);
    GwConnSetNonBlock(s);
    GwLuaInitEnv();
    
    // master
    GwMaster *master = GwMasterInit();
    int id = master->ret;
    
    int epollfd = initKqueue();
    
    if (id < 0) {
        quit("GwMasterInit()");
    } else if (id == 0) {
        printf("chlid %d %d\n", getpid(), s);
        GwWorkerRun(s);
    } else {
        // todo monitor
        printf("parent %d\n", getpid());
        int wpid, status;
        while ((wpid = wait(&status)) > 0) {
            int i = WEXITSTATUS(status); // 进程的返回值
            int wif = WIFEXITED(status); // 子进程是否为正常退出的，如果是，它会返回一个非零值
            printf("status: %d %d %d\n", i, wif, wpid);
        }
        printf("over master\n");
    }
    
    return 0;
}

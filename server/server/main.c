//
//  main.c
// cc *.c core/*.c -Ilua-5.3.4/src  -llua -Llua-5.3.4/src -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib && ./a.out
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

#include "gwpool.h"
#include "gwstring.h"


int
main(int argc, const char *argv[]) {
    
    // socket
    unsigned int port = 3000;
    GwConnection *conn = GwConnOpenSocket(port);
//    GwConnection *conn = GwConnSSLOpenSocket(3001);
    int s = conn->server;
//    GwConnSetNonBlock(s);
    GwLuaInitEnv();

    // shm
    int shmid = GwShmInit();
    printf("shmid %d\n", shmid);
    GwShm *shm = GwShmat(shmid);
    GwShmMutex *mtx = &shm->mutexData;
    mtx->num = 0;
    GwMutexCreate(mtx, EnumGwMutexShared);

    // master
    GwMaster *master = GwMasterInit();
    master->shm = shm;

    int id = master->ret;
    if (id < 0) {
        GwShmRemove(shmid);
        quit("GwMasterInit()");

    } else if (id == 0) {
        printf("chlid %d %d\n", getpid(), s);
//        GwMasterStartWorker(master, conn);
        // GwWorkerRun(s);
        GwShmdt(shm);

    } else {
        // todo monitor
        printf("parent %d\n", getpid());
        int wpid, status;
        while ((wpid = wait(&status)) > 0) {
            int i = WEXITSTATUS(status);    // 进程的返回值
            int wif = WIFEXITED(status);    // 子进程是否为正常退出的，如果是，它会返回一个非零值
            printf("status: %d %d %d\n", i, wif, wpid);
        }

        GwConnSSLFree(conn);
        GwLuaCloseEnv();
        GwMutexDestroy(mtx);
        GwShmRemove(shmid);
        
        GwPool *pool = master->pool;
        GwPoolLogger(pool);
        GwPoolDestroy(pool);
    }

    return 0;
}

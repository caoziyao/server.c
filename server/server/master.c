//
//  master.c
//  server
//

#include "master.h"


void
_WorkerAction (GwMaster *master, GwConnection *conn) {
//    int s = socketFile;
    GwShm *shm = master->shm;
    
    int shmid = shm->shmid;
    GwWorkerRun(shmid, conn);
}


GwMaster *
GwMasterInit() {
    int n = NumberOfWorker;
    pid_t id = -1;
    
    GwPool *pool = GwPoolCreate(sizeof(GwMaster));
//    GwMaster *m = malloc(sizeof(GwMaster));
    GwMaster *m = (GwMaster *)GwPoolPallc(pool, sizeof(GwMaster));
    
    for (int i = 0; i < n; i++) {
        id = fork();
        if (id == 0 || id == -1) {
            break;
        }
        printf("fork id %d\n", id);
        m->workerId[i] = id;
    }
    m->ret = id;
    m->action = _WorkerAction;
    m->pool = pool;
    
    return m;
}


// 启动worker进程
void
GwMasterStartWorker(GwMaster *master, GwConnection *conn) {
    master->action(master, conn);
}



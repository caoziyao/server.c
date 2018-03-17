//
//  master.c
//  server
//

#include "master.h"


void
_WorkerAction (GwMaster *master, int socketFile) {
    int s = socketFile;
    GwShm *shm = master->shm;
    
    int shmid = shm->shmid;
    GwWorkerRun(shmid, s);
}


GwMaster *
GwMasterInit() {
    int n = NumberOfWorker;
    pid_t id = -1;
    
    GwMaster *m = malloc(sizeof(GwMaster));
    
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
    
    return m;
}


// 启动worker进程
void
GwMasterStartWorker(GwMaster *master, int socketFile) {
    master->action(master, socketFile);
}



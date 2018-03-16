//
//  master.c
//  server
//

#include "master.h"


void
GwMasterStart(){
    
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
    
    return m;
}


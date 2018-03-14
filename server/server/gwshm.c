//
//  gwshm.c
//  server
//
//  Created by working on 2018/3/14.
//  Copyright © 2018年 working. All rights reserved.
//
#define TEXT_SZ 2048

#include "gwshm.h"
#define ShmSize 1000


GwShm *
GwShmInit(){
//    GwShm *shm = malloc(sizeof(GwShm));
//    shm->shmflg = 0666 | IPC_CREAT;
//    shm->size = ShmSize;
    
    int size = sizeof(GwShm);
    int flg = 0666 | IPC_CREAT;
    
    // IPC_PRIVATE
    int shmid = shmget(IPC_PRIVATE, size, flg);
    if (shmid < 0) {
        quit("shmget()");
    }
    
    GwShm *shmptr = (GwShm *)shmat(shmid, 0, 0);
    if (shmptr == (void *)-1) {
        quit("shmat()");
    }
    shmptr->size = size;
    shmptr->shmid = shmid;
//    shmptr->data = NULL;
    
//    shm->shmptr = shmptr;
//    shm->shmid = shmid;
    
    return shmptr;
}


//// todo struct
//GwShmData *
//GwShmReadData(GwShm *shm) {
//    char *shmptr = shm->shmptr;
//    GwShmData *data = malloc(sizeof(GwShmData));
//    memcpy(data, shmptr, sizeof(GwShmData));
//
//    return data;
//}
//
//// todo struct
//void
//GwShmWriteData(GwShm *shm, GwShmData *data){
//    char *shmptr = shm->shmptr;
//    memcpy(shmptr, data, sizeof(GwShmData));
//}


void
GwShmTest(){
    int shmid;
    char *shmptr;
    
//    ptr = malloc(ShmSize);
    
    shmid = shmget(IPC_PRIVATE, ShmSize, 0666);
    shmptr = shmat(shmid, 0, 0);
    
    pid_t id = fork();
    if (id < 0) {
        quit("fork()");
    } else if (id == 0) {
        // child
//        char msg[1024];
//        memcpy(msg, shmptr, sizeof(msg));
        GwShmData *data = malloc(sizeof(GwShmData));
        memcpy(data, shmptr, sizeof(GwShmData));
        
        char *msg = data->msg;
        int s = data->s;
        
        printf("chlid %d %d %s\n", getpid(), s, msg);
        
    } else {
        // parent
        GwShmData *data = malloc(sizeof(GwShmData));
        data->msg = "abcd hello";
        data->s = 12;
        
        memcpy(shmptr, data, sizeof(GwShmData));
        wait(NULL);
    }
}

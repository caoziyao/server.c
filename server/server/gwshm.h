//
//  gwshm.h
//  server


#ifndef gwshm_h
#define gwshm_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "config.h"
#include "utils.h"

typedef struct _GwShm GwShm;
typedef struct _GwShmData GwShmData;

struct _GwShmData {
    int s;
    char *msg;
    int option;
    struct sockaddr_in serveraddr;
    int child[NumberOfWorker];
    int n;
};

struct _GwShm {
    //    char *shmptr;
    int shmflg;  // shmflg是权限标志
    int size;
    int shmid;
    GwShmData data;
};

// 共享内存初始化
GwShm *
GwShmInit();

// todo struct
GwShmData *
GwShmReadData(GwShm *shm);

// todo struct
void
GwShmWriteData(GwShm *shm, GwShmData *data);


// test
void
GwShmTest();



#endif /* gwshm_h */

//
//  gwshm.h
//  server

#ifndef gwshm_h
#define gwshm_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "utils.h"
#include "gwmutex.h"

typedef struct _GwShm GwShm;

struct _GwShm {
    //    char *shmptr;
    int shmflg;  // shmflg是权限标志
    int size;
    int shmid;
    GwShmMutex mutexData;
};

// 共享内存初始化
int
GwShmInit();

// 把共享内存区对象映射到调用进程的地址空间
GwShm *
GwShmat(int shmid);

// 断开共享内存连接
void
GwShmdt(GwShm *shm);

// 共享内存管理
void
GwShmctl(int shmid, int cmd, struct shmid_ds *buf);


// 删除shm, 等价于GwShmctl(shmid, IPC_RMID, NULL);
void
GwShmRemove(int shmid);


//// todo struct
//GwShmData *
//GwShmReadData(GwShm *shm);
//
//// todo struct
//void
//GwShmWriteData(GwShm *shm, GwShmData *data);


// test
int
GwShmTest();



#endif /* gwshm_h */

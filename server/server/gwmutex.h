//
//  gwmutex.h
//  server
//
//  Created by working on 2018/3/17.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwmutex_h
#define gwmutex_h

#include <stdio.h>
#include <pthread.h>

typedef struct _GwShmMutex GwShmMutex;
struct _GwShmMutex {
    int num;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};

typedef enum _EnumGwMutex EnumGwMutex;
enum _EnumGwMutex {
    EnumGwMutexShared = 1,
    EnumGwMutexPrivate = 2
};

// 创建一个互斥量
void
GwMutexCreate(GwShmMutex *mtx, EnumGwMutex status);

// 锁一个互斥量
void
GwMutexLock(GwShmMutex *mtx);

// 打开一个互斥量
void
GwMutexUnlock(GwShmMutex *mtx);

// 销毁互斥量
void
GwMutexDestroy(GwShmMutex *mtx);

// 等待互斥量对象
void
GwMutexWait();

#endif /* gwmutex_h */

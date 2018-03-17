//
//  gwmutex.c
//  server
//
//  Created by working on 2018/3/17.
//  Copyright © 2018年 working. All rights reserved.
//

#include "gwmutex.h"


void
GwMutexCreate(GwShmMutex *mtx, EnumGwMutex status) {
    // 初始化mutex属性对象
    pthread_mutexattr_init(&mtx->mutexattr);
    // 修改属性为进程间共享
    pthread_mutexattr_setpshared(&mtx->mutexattr, status);
    // 初始化一把mutex琐
    pthread_mutex_init(&mtx->mutex, &mtx->mutexattr);
}


// 锁一个互斥量
void
GwMutexLock(GwShmMutex *mtx) {
    pthread_mutex_lock(&mtx->mutex);
}


// 打开一个互斥量
void
GwMutexUnlock(GwShmMutex *mtx) {
    pthread_mutex_unlock(&mtx->mutex);
}


// 销毁互斥量
void
GwMutexDestroy(GwShmMutex *mtx) {
    pthread_mutexattr_destroy(&mtx->mutexattr);          //销毁mutex属性对象
    pthread_mutex_destroy(&mtx->mutex);                  //销毁mutex
}




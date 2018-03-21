//
//  worker.c
//  server
//
/*
 惊群：
 利用一把进程间锁，每个进程中都尝试获得这把锁，
 如果获取成功将监听socket加入wait集合中，并设置超时等待连接到来，
 没有获得所的进程则将监听socket从wait集合去除
 */

#include "worker.h"

static int fd;
static int s;
GwShmMutex *mtx;

void *
signal_handler(int n)
{
    (mtx->num)++;
    printf("-child----------num++ %d  %d\n", getpid(), mtx->num);

    // 注册事件，让每个进程轮流监听 socket
    //1、获得锁则加入wait集合
    //2、没有获得则去除
    GwMutexLock(mtx);
    GwKqueueAddListener(fd, s);
    GwMutexUnlock(mtx);
    return NULL;
}


void
GwWorkerRun(int shmid, GwConnection *conn) {
    
    // todo 更好的方案
    GwPool *pool = GwPoolCreate(sizeof(GwWorker));
    GwWorker *w = (GwWorker *)GwPoolPallc(pool, sizeof(GwWorker));
    
    w->pool = pool;
    
    // GwShm *shm = GwShmat(shmid);
    // mtx = &shm->mutexData;
    int s = conn->server;
    
    fd = initKqueue();
    
    // GuaThreadPool *pool = GuaThreadPoolNew(2);
    // GuaThreadPoolAddTask(pool, response, n);
    // GwTimerStart(1, signal_handler);
    GwKqueueAddListener(fd, s);

    struct kevent events[MaxEventCount];
    while (true) {
        int ret = kevent(fd, NULL, 0, events, MaxEventCount, NULL);
        printf("ret %d\n", ret);
        GwKqueueHandleEvent(fd, events, ret, conn);
    }
    
    GwPoolDestroy(pool);
}

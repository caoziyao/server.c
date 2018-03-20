//
//  threadpool.h
//
//  Created by working on 2018/1/12.
//  Copyright © 2018年 working. All rights reserved.


#ifndef guathreadpool_h
#define guathreadpool_h

#include <stdio.h>

#include <pthread.h>


struct _GuaThreadPool;
typedef struct _GuaThreadPool GuaThreadPool;

// 函数指针 typedef
typedef void *(GuaTaskCallback)(void *);


// 创建并返回一个 thread pool
// 参数为线程池的线程数量
GuaThreadPool *
GuaThreadPoolNew(int numberOfThreads);

// 给线程池添加一个任务
// pool 是线程池
// callback 是线程要执行的函数
// arg 是传给
int
GuaThreadPoolAddTask(GuaThreadPool *pool, GuaTaskCallback *callback, void *arg);

// 删除并释放一个线程池的资源
// pool 是要销毁的线程池
int
GuaThreadPoolFree(GuaThreadPool *pool);

void
threadRun();

#endif /* guathreadpool_h */

//
//  threadpool.c
//
//  Created by working on 2018/1/12.
//  Copyright © 2018年 working. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "threadpool.h"

struct _Task;
typedef struct _Task GuaTask;

static pthread_mutex_t mutex;
static pthread_cond_t cv;
pthread_condattr_t   *cond_attr = NULL;


struct _TaskQueue;
typedef struct _TaskQueue GuaTaskQueue;
// GuaTaskQueue 的定义和接口你自己来根据需要补全
struct _TaskQueue{
    int n;
    GuaTask *task;
};


struct _Task {
    GuaTaskCallback *callback;
    void *arg;                      //回调函数参数
    GuaTask *next;
};


struct _GuaThreadPool {
    int numberOfThreads;        // 线程池中的线程数量
    pthread_t threads[5];         // 线程池中所有线程的 id
    // 补全你需要的其他变量
    GuaTaskQueue *queue;
};


// 线程池里面的单个线程的入口函数
void *_GuaThreadPoolThreadNew(void *arg);


GuaThreadPool*
GuaThreadPoolNew(int numberOfThreads) {
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cv, NULL);
    
    int n = numberOfThreads;
    
    GuaThreadPool *tp = (GuaThreadPool *)malloc(sizeof(GuaThreadPool));
    tp->numberOfThreads = n;
    
    GuaTaskQueue *queue = (GuaTaskQueue *)malloc(sizeof(GuaTaskQueue));
    
    GuaTask *task = (GuaTask *)malloc(sizeof(GuaTask));
    task->callback = NULL;
    task->next = NULL;
    task->arg = NULL;
    
    queue->task = task;
    
    queue->n = 0;
    tp->queue = queue;
    
    for (int i = 0; i < n; i++) {
        int err = pthread_create(&tp->threads[i], NULL, _GuaThreadPoolThreadNew, tp);
        if (err != 0) {
            printf("create pthread error: %s\n", strerror(err));
            return NULL;
        }
    }
    printf("create over\n");
    return tp;
}


GuaTask *
appendTaskFromQueue(GuaTaskQueue *queue, GuaTask *newTask) {
    
    GuaTask *task = queue->task;
    
    while (task->next != NULL) {
        task = task->next;
    }
    task->next = newTask;
    return task;
}


int
GuaThreadPoolAddTask(GuaThreadPool* pool, GuaTaskCallback *callback, void *arg) {
    
    GuaTaskQueue *queue = pool->queue;
    
    GuaTask *task = (GuaTask *)malloc(sizeof(GuaTask));
    task->callback = callback;
    task->next = NULL;
    task->arg = arg;
    
    appendTaskFromQueue(queue, task);
    
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mutex);
    
    queue->n++;
    
    return 0;
}


bool
isEmptyGuaTaskQueue(GuaTaskQueue *queue) {
    int n = queue->n;
    if (n == 0) {
        return true;
    } else {
        return false;
    }
}


GuaTask *
popGuaTaskQueue(GuaTaskQueue *queue) {
    
    GuaTask *head = queue->task;
    GuaTask *t = head->next;
    
    GuaTask *task = head->next;
    if (task != NULL) {
        head->next = task->next;
        queue->n--;
    }
    
    return t;
}

bool
isEmptyTask(GuaTaskQueue *queue) {
    int n = queue->n;
    if (n == 0) {
        return true;
    } else {
        return false;
    }
}


void *
_GuaThreadPoolThreadNew(void* arg) {
    
    GuaThreadPool *pool = (GuaThreadPool *)arg;
    sleep(1);
    // 如果队列为空，则阻塞
    while (true) {
        GuaTaskQueue *queue = pool->queue;
        if (isEmptyGuaTaskQueue(queue)) {
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&cv, &mutex);
            pthread_mutex_unlock(&mutex);
        }
        else {
            
            GuaTask *task = popGuaTaskQueue(queue);
            if (task != NULL) {
                
                void * args = task->arg;
                task->callback(args);
            }
            
//            if (isEmptyTask(queue)) {
//                //                exit(0);
//                printf("task over\n");
//                break;
//            }
        }
    }
    
    return NULL;
}


int
GuaThreadPoolFree(GuaThreadPool *pool) {
    int n = pool->numberOfThreads;
    
    for (int i = 0; i < n; i++) {
        pthread_t t = pool->threads[i];
        //        printf("join %u\n", (unsigned int)t);
        pthread_join(t, NULL);
        
    }
    pthread_cond_destroy(&cv);
    return 0;
}




//void
//threadTestRun(){
//    const int numberOfTasks = 20;
//
//    GuaThreadPool *pool = GuaThreadPoolNew(5);
//
//    unsigned int port = 3000;
//    int s = openSocket(port);
//
//    struct sockaddr_in client;
//    int size = sizeof(struct sockaddr_in);
//
//    while (true) {
//        int *n = malloc(sizeof(int));
//        *n = accept(s, (struct sockaddr *)&client, (socklen_t *)&size);
//        GuaThreadPoolAddTask(pool, GuaResponse, n);
//        printf("(add task %d)\n", *n);
//    }
//
//    // GuaThreadPoolFree 要等所有的线程都退出后才会返回
//    // 因为在队列中还没有执行的函数会执行后再退出
//    GuaThreadPoolFree(pool);
//}



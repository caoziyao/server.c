//
//  gwkqueue.h
//  server

#ifndef gwkqueue_h
#define gwkqueue_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "utils.h"
#include "config.h"
#include "constant.h"
#include "threadpool.h"

int
initKqueue();

// 注册事件到 kqueue  Register
bool
GwKqueueAddListener(int kq, int fd);

// 删除事件
bool
GwKqueueRemoveListener(int kq, int fd);

// 等待内核事件通知
void
GwKqueueWaitEvent(int kq);

// 处理事件
void
GwKqueueHandleEvent(int kq, struct kevent* events, int nevents, int socketFile);


#endif /* guakqueue_h */

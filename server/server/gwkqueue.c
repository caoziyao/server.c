//
//  gwkqueue.c
//  server


#include "gwkqueue.h"
#include "gwsocket.h"

const static int FdNum = 2;         // 两个文件描述符


int
initKqueue(){
    int fd = kqueue();
    if (fd < 0) {
        quit("kevent()");
    }
    return fd;
}


int
updateEventsType(struct kevent *ev, int type,  int n, int fd, GwKQueueFilter filter, GwKQueueFlag flag){
    
    if ((flag & GwKQueueFlagAdd) || (flag & GwKQueueFlagEnable)) {
        EV_SET(&ev[n++], fd, type, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    } else if (flag & GwKQueueFlagDelete) {
        EV_SET(&ev[n++], fd, type, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    } else {
        quit("unknow updateEvents() flag ");
    }
    return n;
}


void
updateEvents(int efd, int fd, GwKQueueFilter filter, GwKQueueFlag flag) {
    struct kevent ev[FdNum];
    int n = 0;
    int type;
    
    if (filter & GwKQueueFilterRead) {
        type = EVFILT_READ;
        n = updateEventsType(ev, type,  n, fd, filter, flag);
    }
    
    if (filter & GwKQueueFilterWrite) {
        type = EVFILT_WRITE;
        n = updateEventsType(ev, type,  n, fd, filter, flag);
    }
    
    int r = kevent(efd, ev, n, NULL, 0, NULL);
    if (r < 0) {
        quit("updateEvents()");
    }
}


static int a = 0;
void
loopOnce(int efd, int lfd, int waitms, GuaThreadPool *pool){
    struct timespec timeout;
    timeout.tv_sec = waitms / 1000;
    timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;
    
    const int kMaxEvents = 20;
    struct kevent activeEvs[kMaxEvents];
    
    // 进行kevent函数调用
    int n = kevent(efd, NULL, 0, activeEvs, kMaxEvents, &timeout);
//    printf("epoll_wait %d %d\n", n, a);
    
    for (int i = 0; i < n; i++) {
        // 一个个取出已经就绪的事件
        struct kevent event = activeEvs[i];
        if( event.flags & EV_ERROR ){
            quit("Event error");
        }
        
        // 从附加数据里面取回文件描述符的值
        int ev_fd = (int)(intptr_t)event.udata;
        int filter = event.filter;
        if (filter == EVFILT_READ) {
            if (ev_fd == lfd) {
                // accept
                int cfd = handleAccept(ev_fd);
                setNonBlock(cfd);
                updateEvents(efd,  cfd, GwKQueueFilterRead | GwKQueueFilterWrite, GwKQueueFlagAdd);
            } else {
                // read
                handleRead(ev_fd);
            }
        }  else if (filter == EVFILT_WRITE) {
            a++;
            // write
            response(&ev_fd);
//            GuaThreadPoolAddTask(pool, response, (void *)&ev_fd);
            // 实际应用应当实现可写时写出数据，无数据可写才关闭可写事件
            // updateEvents(efd, ev_fd, GwKQueueFilterWrite, GwKQueueFlagDelete);
            
        } else {
            printf("unknow event");
        }
        
    }
}


//void
//kqueueTest(){
    // kqueue
    //    int epollfd = initKqueue();
    //
    //    // socket
    //    unsigned int port = 3000;
    //    int s = openSocket(port);
    //    setNonBlock(s);
    //
    //    struct sockaddr_in client;
    //    int size = sizeof(struct sockaddr_in);
    //
    //    // 注册事件
    //    updateEvents(epollfd, s, GwKQueueFilterRead | GwKQueueFilterWrite, GwKQueueFlagAdd);
    //
    //    while (true) {
    //        loopOnce(epollfd, s, 10000);
    //    }
//}


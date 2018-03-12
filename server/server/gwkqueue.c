//
//  gwkqueue.c
//  server


#include "gwkqueue.h"
#include "gwsocket.h"

const static int FdNum = 2;         // 两个文件描述符
const static int BufferSize = 1024; // 缓冲区大小
static int kReadEvent = 1;
static int kWriteEvent = 2;
static int kAdd = 1;
static int kDelete = 2;

int
initKqueue(){
    int fd = kqueue();
    if (fd < 0) {
        quit("kevent()");
    }
    return fd;
}


void
initEvent(int efd, int fd){
    struct kevent ev[FdNum];
    int n = 0;
    EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    
    int r = kevent(efd, ev, n, NULL, 0, NULL);
    if (r < 0) {
        quit("updateEvents()");
    }
}


void
updateEvents(int efd, int fd, int event, int status){
    struct kevent ev[FdNum];
    int n = 0;
    
    // 注册事件
    if (event & kReadEvent) {
        // read
        if (status & kAdd) {
            EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
        } else if (status & kDelete) {
            EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
        } else {
            quit("updateEvents read()");
        }
    }
    
    if (event & kWriteEvent) {
        // write
        if (status & kAdd) {
            EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
        } else if (status & kDelete) {
            EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
        } else {
            quit("updateEvents write()");
        }
    }
    
    int r = kevent(efd, ev, n, NULL, 0, NULL);
    if (r < 0) {
        quit("updateEvents()");
    }
}

void
handleAccept(int efd, int fd){
    struct sockaddr_in raddr;
    socklen_t rsz = sizeof(raddr);
    int cfd = accept(fd,(struct sockaddr *)&raddr,&rsz);
    struct sockaddr_in peer, local;
    socklen_t alen = sizeof(peer);
    
    int r = getpeername(cfd, (struct sockaddr*)&peer, &alen);
    setNonBlock(cfd);
    
//    updateEvents(efd, cfd, kReadEvent | kWriteEvent, kAdd);
    initEvent(efd, cfd);
}

void
loopOnce(int efd, int lfd, int waitms){
    struct timespec timeout;
    timeout.tv_sec = waitms / 1000;
    timeout.tv_nsec = (waitms % 1000) * 1000 * 1000;
    
    const int kMaxEvents = 20;
    struct kevent activeEvs[kMaxEvents];
    
    // 进行kevent函数调用
    int n = kevent(efd, NULL, 0, activeEvs, kMaxEvents, &timeout);
    printf("epoll_wait return %d\n", n);
    char buffer[BufferSize];
    
    for (int i = 0; i < n; i++) {
        // 一个个取出已经就绪的事件
        struct kevent event = activeEvs[i];
        if( event.flags & EV_ERROR ){
            quit("Event error");
        }
        // 从附加数据里面取回文件描述符的值
        int ev_fd = (int)(intptr_t)event.udata;
//        int ev_fd = *((int *)event.udata);
        int filter = event.filter;
        if (filter == EVFILT_READ) {
            
            if (ev_fd == lfd) {
                // accept
                printf("accept\n");
                handleAccept(efd, ev_fd);
            } else {
                // read
                printf("read\n");
                read(ev_fd, buffer, sizeof(buffer));
                printf("buffer %s\n", buffer);
                close(ev_fd);
            }
        }  else if (filter == EVFILT_WRITE) {
            // write
            printf("write\n");
            //实际应用应当实现可写时写出数据，无数据可写才关闭可写事件
            updateEvents(efd, ev_fd, kWriteEvent, kDelete);
            
        } else {
            printf("unknow event");
        }
        
    }
    
    
}
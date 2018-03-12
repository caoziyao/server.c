//
//  input_kqueue.c
//  server
//
//  Created by working on 2018/3/12.
//  Copyright © 2018年 working. All rights reserved.
// kqueue

#include "input_kqueue.h"

/*
 //类似epoll_create
 int kqueue(void);
 //组合了epoll_ctl及epoll_wait功能，changelist与nchanges为修改列表，eventlist与nevents为返回的事件列表
 int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout);
 //设定kevent参数的宏，详细解释见后面的调用示例
 EV_SET(&kev, ident, filter, flags, fflags, data, udata);
 
 */

const static int FD_NUM = 2; // 两个文件描述符，分别为标准输入与输出
const static int BUFFER_SIZE = 1024; // 缓冲区大小

// 错误退出的工具函数
int quit(const char *msg){
    perror(msg);
    exit(1);
}

// 为文件描述符打开对应状态位的工具函数
void turn_on_flags(int fd, int flags){
    int current_flags;
    // 获取给定文件描述符现有的flag
    // 其中fcntl的第二个参数F_GETFL表示要获取fd的状态
    if( (current_flags = fcntl(fd, F_GETFL)) < 0 ) exit(1);
    
    // 施加新的状态位
    current_flags |= flags;
    if( fcntl(fd, F_SETFL, current_flags) < 0 ) exit(1);
}


int
inputKqueue() {
    
    // 完全以IO复用的方式读入标准输入流数据，输出到标准输出流中
    struct kevent changes[FD_NUM];  // 要监视的事件列表
    struct kevent events[FD_NUM];  // kevent返回的事件列表（
    
    // 创建一个kqueue，与epoll类似
    int kq;
    if( (kq = kqueue()) == -1 )  {
        quit("kqueue()");
    }
    
    // 准备从标准输入流中读数据
    int stdin_fd = STDIN_FILENO;
    int stdout_fd = STDOUT_FILENO;
    
    // 设置为非阻塞
    turn_on_flags(stdin_fd, O_NONBLOCK);
    turn_on_flags(stdout_fd, O_NONBLOCK);
    
    // 注册监听事件
    int k = 0;
    // 在changes列表中注册标准输入流的读事件 以及 标准输出流的写事件
    EV_SET(&changes[k++], stdin_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &stdin_fd);
    EV_SET(&changes[k++], stdout_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &stdout_fd);
    
    int nev, nread, nwrote = 0; // 发生事件的数量, 已读字节数, 已写字节数
    char buffer[BUFFER_SIZE];
    
    while (true) {
        // 进行kevent函数调用，如果changes列表里有任何就绪的fd，则把该事件对应的结构体放进events列表里面
        // nev 已经就绪的文件描述符数量
        nev = kevent(kq, changes, FD_NUM, events, FD_NUM, NULL);
        if( nev <= 0 ) {
            quit("kevent()");
        }
        
        int i;
        for(i=0; i<nev; i++){
            // 一个个取出已经就绪的事件
            struct kevent event = events[i];
            if( event.flags & EV_ERROR ){
                quit("Event error");
            }
            // 从附加数据里面取回文件描述符的值
            int ev_fd = *((int *)event.udata);
            
            // 输入流就绪 且 缓冲区还有空间能继续读
            if( ev_fd == stdin_fd && nread < BUFFER_SIZE ){
                 // 读取ready_fd
                int new_nread;
                if( (new_nread = read(ev_fd, buffer + nread, sizeof(buffer) - nread)) <= 0 ){
                    quit("read()"); // 由于可读事件已经发生，因此如果读出0个字节也是不正常的
                }
                // 递增已读数据字节数
                nread += new_nread;
                printf("read %d\n", nread);
            }
            // 输出流就绪 且 缓冲区有内容可以写出
            if( ev_fd == stdout_fd && nread > 0 ){
                // 写入ready_fd
                if( (nwrote = write(stdout_fd, buffer, nread)) <=0 ){
                    quit("write()");
                }
                // 为了使实现的代码更简洁，这里把还没有写出去的数据往前移动
                memmove(buffer, buffer+nwrote, nwrote);
                nread -= nwrote; // 减去已经写出去的字节数
            }
        }
    }
    
    return 0;
}

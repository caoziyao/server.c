//
//  keyboard.c
//  server
//
//  Created by working on 2018/3/12.
//  Copyright © 2018年 working. All rights reserved.
//

#include "keyboard.h"

// 读取键盘输入值，超时间隔2.5秒，输出用户输入的字符个数
void
keyBoardSelect() {
    char buffer[128];
    int result, nread;
    
    // struct fd_set可以理解为一个集合，这个集合中存放的是文件描述符(filedescriptor)，即文件句柄，
    // 这可以是我们所说的普通意义的文件
    fd_set inputs, testfds;
    
    struct timeval timeout;
    
    // 清空集合FD_ZERO(fd_set *)
    FD_ZERO(&inputs);
    // 将一个给定的文件描述符加入集合之中FD_SET(int ,fd_set*)
    FD_SET(0, &inputs);
    // 将一个给定的文件描述符从集合中删除FD_CLR(int,fd_set*)
    
    while (true) {
        testfds = inputs;
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;
        
        // 正值：某些文件可读写或出错
        result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, &timeout);
        
        switch (result) {
            case 0:
                printf("timeout\n");
                break;
            case -1:
                printf("-1 select\n");
                exit(1);
                break;
            default:
                // 检查集合中指定的文件描述符是否可以读写FD_ISSET(int ,fd_set* )
                if (FD_ISSET(0,&testfds)) {
                    ioctl(0, FIONREAD, &nread);//取得从键盘输入字符的个数，包括回车。
                    if (nread == 0) {
                        printf("keyboard done\n");
                        exit(0);
                    }
                    nread = read(0, buffer, nread);
                    buffer[nread] = 0;
                    printf("read %d from keyboard: %s", nread, buffer);
                }
                break;
        }
    }
}

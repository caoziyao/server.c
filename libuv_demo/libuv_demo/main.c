//
//  main.c
//  libuv_demo
//
//  Created by working on 2018/1/20.
//  Copyright © 2018年 working. All rights reserved.
//  /usr/local/Cellar/libuv/1.19.0/include
// gcc *.c -I/usr/local/Cellar/libuv/1.19.0/include -L/usr/local/Cellar/libuv/1.19.0/lib -luv && ./a.out

/*
 事件驱动编程
 libuv会负责将来自操作系统的事件收集起来，或者监视其他来源的事件
 用户就可以注册回调函数，回调函数会在事件发生的时候被调用
 
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
    counter++;
    
    if (counter >= 10e6)
        uv_idle_stop(handle);
}

int main() {
    uv_idle_t idler;
    
    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, wait_for_a_while);
    
    printf("Idling...\n");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    
    uv_loop_close(uv_default_loop());
    return 0;
}

//
//  gwtime.c
//  server
//
//  Created by working on 2018/3/17.
//  Copyright © 2018年 working. All rights reserved.
//
/*
 todo
 对定时器的封装
 时间轮重写
 */
#include "gwtimer.h"

static struct itimerval oldtv;

int
GwTimerStart(int interval, void *action) {
    
    signal(SIGALRM, action);  //注册当接收到SIGALRM时会发生是么函数；
    
    struct itimerval itv;
    // 表示时间间隔
    itv.it_interval.tv_sec = interval;  //设置为1秒
    itv.it_interval.tv_usec = 0;
    // 表示第一次定的时间
    itv.it_value.tv_sec = 0;    // interval
    itv.it_value.tv_usec = 1;
    setitimer(ITIMER_REAL, &itv, &oldtv);  //此函数为linux的api,不是c的标准库函数
    
    return 0;
}


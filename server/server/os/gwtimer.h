//
//  gwtimer.h
//  server
//
//  Created by working on 2018/3/17.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwtimer_h
#define gwtimer_h

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

// 定时到期函数
typedef void (*GwTimerExpiryAction)(int n);

// 注册一个时间间隔为 Interval 后执行 ExpiryAction 的定时器实例
// 返回 TimerId
int
GwTimerStart(int interval, void *action);

// 根据 timerId 找到注册的定时器实例并执行 Stop
void
GwTimerStop(int timerId);

// 在一个 Tick 时间粒度内，定时器系统需要执行的动作，
// 它最主要的行为，就是检查定时器系统中，是否有定时器实例已经到期。
void
GwTimerPerTickBookkeeping();

// 在定时器实例到期之后，执行预先注册好的 ExpiryAction 行为。
void
GwTimerExpiryProcessing();


#endif /* gwtimer_h */

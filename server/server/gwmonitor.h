//
//  gwmonitor.h
//  server
//
//  Created by working on 2018/3/15.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwmonitor_h
#define gwmonitor_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "config.h"

typedef struct _GwMonitor GwMonitor;
struct _GwMonitor{
    int chlidId[NumberOfWorker];
    int n;
};

GwMonitor *
GwMonitorInit();

void
GwMonitorAddPid(GwMonitor *monitor, int pid);

void
GwMonitorRun(GwMonitor *monitor);


#endif /* gwmonitor_h */

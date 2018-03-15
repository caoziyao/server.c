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

typedef struct _GwMonitorData{
    int chlidId[NumberOfWorker];
    int n;
}GwMonitorData;

void
GwMonitorAddPid(GwMonitorData *data, int pid);

void
GwMonitorRun(GwMonitorData *data);


#endif /* gwmonitor_h */

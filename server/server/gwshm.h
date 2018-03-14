//
//  gwshm.h
//  server
//
//  Created by working on 2018/3/14.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwshm_h
#define gwshm_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

#include "config.h"
#include "utils.h"

typedef struct _GwShm GwShm;
typedef struct _GwShmData GwShmData;

struct _GwShmData {
    int s;
    char *msg;
};


// 共享内存初始化
GwShm *
GwShmInit();

// todo struct
GwShmData *
GwShmReadData(GwShm *shm);

// todo struct
void
GwShmWriteData(GwShm *shm, GwShmData *data);


// test
void
GwShmTest();



#endif /* gwshm_h */

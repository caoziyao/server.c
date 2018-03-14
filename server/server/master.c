//
//  master.c
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

#include "master.h"


void
GwMasterStart(){
    
}


void
GwMasterFork(pid_t *pid) {
    int n = NumberOfWorker;
    
    pid_t *id = pid;
    for (int i = 0; i < n; i++) {
        *id = fork();
        if (*id == 0 || *id == -1) {
            break;
        }
    }
}

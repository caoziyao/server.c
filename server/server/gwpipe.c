//
//  gwpipe.c
//  server
//
//  Created by working on 2018/3/14.
//  Copyright © 2018年 working. All rights reserved.
//

#include "gwpipe.h"
#include "config.h"
#include "utils.h"

int *
GwPipeInit(){
    int *p = (int *)malloc(sizeof(int) * 2);
    int err = pipe(p);
    if (err < 0) {
        quit("pipe()");
    }
    
    return p;
}


void
GwPipeClose(int *fd, GwPipeFdEnd end){
    close(fd[end]);
}


char *
GwPipeRead(int *fd) {
    // todo 动态string
    char *msg = malloc(sizeof(char) * 256);
    memset(msg, '\0', 256);
    
    int r = fd[EGwPipeRead];
    read(r, msg, 256);
    
    return msg;
}


void
GwPipeWrite(int *fd, char *msg, int n) {
    if (msg != NULL) {
        int w = fd[EGwPipeWrite];
        write(w, msg, n);
    }
}


void
GwPipeTest() {
    int p[2];
    //    pipefd [0]指向管道的读端, pipefd [1]指向管道的写端
    int ret = pipe(p);
    if (ret < 0) {
        quit("pipe()");
    }
    
    pid_t id = fork();
    if (id < 0) {
        quit("fork()");
    } else if (id == 0) {
        // child
        close(p[0]);
        int i = 0;
        char *msg = NULL;
        while (i < 100) {
            msg = "I am child";
            write(p[1], msg, strlen(msg));
            sleep(1);
            i++;
        }
        
    } else {
        // parent
        close(p[1]);
        int j = 0;
        char rsp[100];
        while (j < 100) {
            memset(rsp, '\0', sizeof(rsp));
            read(p[0], rsp, sizeof(rsp));
            printf("parent: %s\n", rsp);
            j++;
        }
    }
}

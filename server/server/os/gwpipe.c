//
//  gwpipe.c
//  server
//
//  Created by working on 2018/3/14.
//  Copyright © 2018年 working. All rights reserved.
//

#include "gwpipe.h"

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


void
GwPipeRead(int *fd, char *msg, int n) {
    memset(msg, '\0', n);
    int r = fd[EGwPipeRead];
    read(r, msg, n);
}


void
GwPipeWrite(int *fd, char *msg, int n) {
    if (msg != NULL) {
        int w = fd[EGwPipeWrite];
        write(w, msg, n);
    }
}

// todo struct
void
GwPipeReadData(int *fd, void *s) {
//    GwPipeData *data = (GwPipeData *)s;
    int r = fd[EGwPipeRead];
    read(r, s, sizeof(GwPipeData));
}


// todo struct
void
GwPipeWriteData(int *fd, void *s) {
//    GwPipeData *data = (GwPipeData *)s;
    int w = fd[EGwPipeWrite];
    write(w, s, sizeof(GwPipeData) + 1);
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

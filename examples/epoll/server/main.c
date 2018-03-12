//
//  main.c
//
//  Created by working on 2018/1/19.
//  Copyright © 2018年 working. All rights reserved.
// cc *.c -Ilua-5.3.4/src -llua -Llua-5.3.4/src && ./a.out


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "keyboard.h"
#include "server_select.h"
#include "input_kqueue.h"

int
main(int argc, const char *argv[]) {

    inputKqueue();
    
    return 0;
}

//
//  gwkqueue.h
//  server

#ifndef gwkqueue_h
#define gwkqueue_h

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

#include "utils.h"
#include "config.h"

int
initKqueue();

void
updateEvents(int efd, int fd, int event, int status);

void
initEvent(int efd, int fd);

#endif /* guakqueue_h */

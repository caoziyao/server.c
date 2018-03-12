//
//  gwsocket.h
//  server
//

#ifndef gwsocket_h
#define gwsocket_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int
openSocket(unsigned short port);

void
setNonBlock(int fd);

#endif /* gwsocket_h */

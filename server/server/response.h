//
//  response.h
//  server
//
//  Created by working on 2018/3/12.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef response_h
#define response_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


void *
GuaResponse(void *socketFile);



#endif /* response_h */

//
//  redis.h
//  cache
//
//  Created by working on 2018/4/10.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef redis_h
#define redis_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dict.h"

typedef struct redisDbStruct redisDb;
typedef struct redisServerStruct redisServer;
typedef struct redisClientStruct redisClient;

// 数据库初始化
redisServer *
redisServerInit();

void
redisServerTest();

#endif /* redis_h */

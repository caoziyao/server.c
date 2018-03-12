//
//  utils.c
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

#include "utils.h"


// 错误退出的工具函数
int
quit(const char *msg){
    perror(msg);
    exit(1);
}

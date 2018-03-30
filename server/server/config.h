//
//  config.h
//  server
//
//  Created by working on 2018/3/13.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef config_h
#define config_h

#include <stdlib.h>

#define NumberOfWorker  2       // worker进程数
#define MaxEventCount   5000    // kqueue event 数
#define MaxRecvBuff     65535   // request 最大接收

#define EnableHttpSSL   ssl    // 支持 ssl

//宏前面加上##的作用在于：当可变参数的个数为0时，这里的## 起到把前面多余的","去掉的作用
#define log(format, ...) printf("[logging] %s(%d): \n%s", \
                                __FUNCTION__, __LINE__, ##__VA_ARGS__);

//#define DEBUG_INFO(format, ...) printf("File:%s, Line:%d, Function:%s, %s", \
//__FILE__, __LINE__ , __FUNCTION__, ##__VA_ARGS__);
#endif /* config_h */

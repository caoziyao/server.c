//
//  gwpool.h
//  server
//
//  Created by working on 2018/3/20.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwpool_h
#define gwpool_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constant.h"

////////// struct //////////

// 析构函数，挂载内存释放时需要清理资源的一些必要操作
typedef void (*GwPoolCleanupPt)(void *data);

// 内存分配相关的日志记录
typedef void (*GwPoolLog)(void);

// ngx_pool_t 内存池头部结构
typedef struct _GwPool GwPool;

// ngx_pool_large_t
typedef struct _GwPoolLarge GwPoolLarge;

// ngx_pool_data_t  内存池数据块结构
typedef struct _GwPoolData GwPoolData;

// cleanup的callback类型
typedef struct _GwPoolCleanup GwPoolCleanup;

////////// api //////////

// 创建一个初始节点大小为size的pool。
GwPool *
GwPoolCreate(size_t size);

// 动态分配内存
void *
GwPoolPallc(GwPool *pool, size_t size);

// 销毁内存池
void
GwPoolDestroy(GwPool *pool);

// 打印内存
void
GwPoolLogger(GwPool *pool);

#endif /* gwpool_h */

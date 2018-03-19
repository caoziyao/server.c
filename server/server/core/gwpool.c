//
//  gwpool.c
//  server
//
//  Created by working on 2018/3/20.
//  Copyright © 2018年 working. All rights reserved.
// todo 内存管理

/*
1，大块内存的分配请求不会直接在内存池上分配内存来满足请求，而是直接向系统申请一块内存（就像直接使用 malloc 分配内存一样），
   然后将这块内存挂到内存池头部的 large 字段下。
2，小块内存分配，则是从已有的内存池数据区中分配出一部分内存。
 */

#include "gwpool.h"

// ngx_pool_data_t 内存池数据块结构
struct _GwPoolData {
    char    *last;      // 当前内存池分配到末位地址，即下一次分配从此处开始。
    char    *end;       // 内存池结束位置
    GwPool  *next;      // 链接到下一个内存池，内存池的很多块内存就是通过该指针连成链表的
};


// ngx_pool_t 内存池的管理分配
struct _GwPool {
    GwPoolData      d;          // 内存池的数据块（上面已有描述），设为d
    size_t          max;        // 数据块大小，小块内存的最大值
    GwPool          *current;   // 指向当前或本内存池
    GwPoolLarge     *large;     // 指向大块内存分配，nginx中，大块内存分配直接采用标准系统接口m
    GwPoolCleanup   *cleanup;   // 析构函数，挂载内存释放时需要清理资源的一些必要操作
    GwPoolLog       *log;       // 内存分配相关的日志记录
};


// ngx_pool_large_t 大块数据分配的结构体
struct _GwPoolLarge {
    GwPoolLarge     *next;
    void            *alloc;
};


// 指向要清除的数据
struct _GwPoolCleanup {
    GwPoolCleanupPt handler;
    void            *data;      // 指向要清除的数据
    GwPoolCleanup   *next;      // 下一个cleanup callback
};

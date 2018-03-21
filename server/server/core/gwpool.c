//
//  gwpool.c
//  server
//
//  Created by working on 2018/3/20.
//  Copyright © 2018年 working. All rights reserved.
// 内存管理

/*
1，大块内存的分配请求不会直接在内存池上分配内存来满足请求，而是直接向系统申请一块内存（就像直接使用 malloc 分配内存一样），
   然后将这块内存挂到内存池头部的 large 字段下。
2，小块内存分配，则是从已有的内存池数据区中分配出一部分内存。
 */

#include "gwpool.h"

#define Max_Alloc_From_Pool 4096


// ngx_pool_data_t 内存池数据块结构
struct _GwPoolData {
    char        *last;      // 当前内存池分配到末位地址，即下一次分配从此处开始。
    char        *end;       // 内存池结束位置
    GwPool      *next;      // 链接到下一个内存池，内存池的很多块内存就是通过该指针连成链表的
    int         failed;     // 记录内存分配不能满足需求的失败次数
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
    GwPoolLarge     *next;      // 指向下一块大块内存
    void            *alloc;     // 指向分配的大块内存
    size_t          max;        // 数据块大小，小块内存的最大值
};


// 指向要清除的数据
struct _GwPoolCleanup {
    GwPoolCleanupPt handler;
    void            *data;      // 指向要清除的数据
    GwPoolCleanup   *next;      // 下一个cleanup callback
};


// todo 清零 GwPool *pool data
static void
_GwMemZero(GwPool *pool, size_t size) {
    memset((void *)pool, 0, size);
}


// 新建节点
static GwPool *
_GwPoolCreateNote(size_t size) {
    int sizeOfPool = sizeof(GwPool);
    size_t max;
    
//    if (size < sizeOfPool) {
//        printf("size < sizeof(GwPool) at GwPoolCreateNote\n");
//        return NULL;
//    }
    
    /* 执行内存池头部 */
    max = (size < Max_Alloc_From_Pool) ? size : Max_Alloc_From_Pool;
    GwPool *p = malloc(max + sizeOfPool);
    _GwMemZero(p, max);
    
    if (p == NULL) {
        perror("GwPoolCreateNote()\n");
        return NULL;
    }
    
    p->d.last = (char *)p + sizeOfPool;
    p->d.end = (char *)p + sizeOfPool + max;
    p->d.next = NULL;
    p->d.failed = 0;
    
    /* 可供分配的空间大小 */
    size = size - sizeof(GwPool);
    /* 不能超过最大的限定值 Max_Alloc_From_Pool */
    p->max = max;
    
    /* 指向当前的内存池 */
    p->current = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = NULL;
    
    return p;
}


/*
 分配一个新的内存池，并将该内存池连接到现有内存池链表中
 同时，返回分配内存的起始地址
 */
static void *
_GwPoolPallcBlock(GwPool *pool, size_t size) {
//    char    *m;
    size_t  psize;
    GwPool  *p, *new, *current;
    GwPoolData *data;
    
    /* 计算pool的大小，即需要分配新的block的大小 */
    psize = (size_t) (pool->d.end - (char *)pool);
    
    /* todo  NGX_POOL_ALIGNMENT对齐操作 m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log); */
    
    /* 计算需要分配的block的大小 */
    new = _GwPoolCreateNote(size);
    
    /* 初始化新的内存池 */
    /* 让m指向该块内存ngx_pool_data_t结构体之后数据区起始位置 */
    p = pool;
    while (p->d.next != NULL) {
        p = p->d.next;
    }
    p->d.next = new;
    
    /* 将分配的block连接到现有的内存池  */
    pool->current = new;
    
    return new;
}


// 大块内存分配函数
static void *
_GwPoolPallcLarge(GwPool *pool, size_t size) {
    void            *p;
    size_t          n;
    GwPoolLarge     *large, *next;
    
    /* 分配内存 */
    large = malloc(size + sizeof(GwPoolLarge));
    if (large == NULL) {
        printf("_GwPoolPallcLarge()\n");
        return NULL;
    }
    p = (void *)(large + sizeof(GwPoolLarge));
    large->next = NULL;
    large->alloc = p;
    large->max = size;
    
    n = 0;
    /* 若在该pool之前已经分配了large字段，
     * 则将所分配的大块内存挂载到内存池的large字段中
     */
    next = pool->large;
    if (next == NULL) {
        pool->large = large;
        return p;
    } else {
        /* 若在该pool之前并未分配large字段，
         * 则执行分配ngx_pool_large_t 结构体，分配large字段内存，
         * 再将大块内存挂载到pool的large字段中
         */
        while (next != NULL) {
            next = large->next;
        }
        large->next = large;
        return p;
    }
}


/*
 创建一个初始节点大小为size的pool。
 并且size的大小必须小于等于NGX_MAX_ALLOC_FROM_POOL，且必须大于sizeof(ngx_pool_t)
 */
GwPool *
GwPoolCreate(size_t size) {
    GwPool *p = _GwPoolCreateNote(size);
    p->current = p;
    return p;
}


// 小内存分配 ngx_palloc
// 请求分配空间 size 小于内存池最大内存值 max
/*
 ngx_palloc 和 ngx_pnalloc 都是从内存池里分配 size 大小内存。他们的不同之处在于，
 palloc 取得的内存是对齐的，pnalloc 则不考虑内存对齐问题。
 ngx_pcalloc 是直接调用 palloc 分配内存，然后进行一次 0 初始化操作。
 ngx_pmemalign 将在分配 size 大小的内存并按 alignment 对齐，然后挂到 large 字段下，当做大块内存处理。
 */
void *
GwPoolPallc(GwPool *pool, size_t size) {
    char    *m;
    GwPool  *p;
    
    /* 若请求的内存大小 size 小于内存池最大内存值max，
     * 则进行小内存分配，从current开始遍历pool链表
     */
    if (size <= pool->max) {
        p = pool->current;
        
        while (p != NULL) {
            /* todo 执行对齐操作，m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT); */
            m = p->d.last;
            
            /* 检查现有内存池是否有足够的内存空间，
             * 若有足够的内存空间，则移动last指针位置，
             * 并返回所分配的内存地址的起始地址
             */
            if ((size_t)(p->d.end - m) >= size ) {
                p->d.last = m + size;   /* 在该节点指向的内存块中分配size大小的内存 */
                return m;
            }
            /* 若不满足，则查找下一个内存池 */
            p = p->d.next;
        }
        
        /* 若遍历所有现有内存池链表都没有可用的内存空间，
         * 则分配一个新的内存池，并将该内存池连接到现有内存池链表中
         * 同时，返回分配内存的起始地址
         */
        return _GwPoolPallcBlock(pool, size);
    }
    
    /* 若所请求的内存大小size大于max则调用大块内存分配函数 */
    return _GwPoolPallcLarge(pool, size);
}


// 销毁内存池
void
GwPoolDestroy(GwPool *pool) {
    GwPool          *p, *n;
    GwPoolLarge     *l, *lpre;
    GwPoolCleanup   *c;
    
    /* 若注册了cleanup，则遍历该链表结构，依次调用handler函数清理数据 */
    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler != NULL) {
            c->handler(c->data);
        }
    }
    
    /* 遍历 large 链表，释放大块内存 */
    l = pool->large;
    while (l != NULL) {
        lpre = l;
        l = l->next;
        free(lpre);
    }
    pool->large = NULL;
    
//    for (l = pool->large; l; l = l->next) {
//        if (l->alloc != NULL) {
//            /* 释放内存 */
//            free(l->alloc);
//        }
//    }
    
    /* 在debug模式下执行 if 和 endif 之间的代码；
     * 主要是用于log记录，跟踪函数销毁时日志信息
     */
    // todo
    
    /* 遍历所有分配的内存池，释放内存池结构 */
    for (p = pool, n = pool->d.next; ; p = n, n = n->d.next) {
        free(p);
        if (n == NULL) {
            break;
        }
    }
    pool = NULL;
}


// 打印内存
void
GwPoolLogger(GwPool *pool) {
    
    GwPoolLarge *large;
    GwPool      *p, *next;
    size_t      max, lmax, unuse;
    char        *last, *end;
    void        *alloc;

    
    p = pool;
    if (p == NULL) {
        printf("pool null\n");
        return;
    }
    
    // print large
    int j = 0;
    large = pool->large;
    while (large != NULL) {
        alloc = large->alloc;
        lmax = large->max;
        printf("%d large:%p alloc:%p max:%zu\n", j, large, alloc, lmax);
        large  = large->next;
        j++;
    }
    
    // print pool
    int     i = 0;
    while(p != NULL) {
        max = p->max;
        next = p->d.next;
        last = p->d.last;
        end = p->d.end;
        unuse = end - last;
        if (next != NULL) {
            printf("%d max:%zu, unuse:%zu, pool_addr:%p, last:%p, end:%p, next:%p\n", i, max, unuse, p, last, end, next);
        } else {
            printf("%d max:%zu, unuse:%zu, pool_addr:%p, last:%p, end:%p, next:NULL\n", i, max, unuse, p, last, end);
        }
        p = p->d.next;
        i++;
    }
    printf("logger over\n");
}


//// pnalloc 则不考虑内存对齐问题
//void *
//GwPoolPnallc(GwPool *pool, size_t size) {
//    return NULL;
//}


////  ngx_pcalloc 直接调用palloc函数，再进行一次0初始化操作
//void *
//GwPoolPcallc(GwPool *pool, size_t size) {
//    void *p;
//    p = GwPoolPallc(pool, size);
//    if (p != NULL) {
//        _GwMemZero(p, size);
//    }
//    return p;
//}


//// 小内存分配 ngx_pmemalign
//void *
//GwPoolPmemalign(GwPool *pool, size_t size, size_t alignment) {
//    return NULL;
//}


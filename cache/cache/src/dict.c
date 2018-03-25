//
//  dict.c
//  cache
//
//  Created by working on 2018/3/25.
//  Copyright © 2018年 working. All rights reserved.
//

#include "dict.h"

typedef struct dictEntryStruct dict_entry;
typedef struct dicthtStruct dictht;
typedef struct dictType dictType;


struct dictEntryStruct {
    // 键
    void *key;
    
    // 值
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
    } v;
    
    // 指向下个哈希表节点，形成链表
    struct dictEntryStruct *next;
};


struct dicthtStruct {
    // 哈希表数组
    dict_entry **table;
    
    // 哈希表大小
    unsigned long size;
    
    // 哈希表大小掩码，用于计算索引值
    // 总是等于 size - 1
    unsigned long sizemask;
    
    // 该哈希表已有节点的数量
    unsigned long used;
};


struct dictType {
    
    // 计算哈希值的函数
    unsigned int (*hashFunction)(const void *key);
    
    // 复制键的函数
    void *(*keyDup)(void *privdata, const void *key);
    
    // 复制值的函数
    void *(*valDup)(void *privdata, const void *obj);
    
    // 对比键的函数
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    
    // 销毁键的函数
    void (*keyDestructor)(void *privdata, void *key);
    
    // 销毁值的函数
    void (*valDestructor)(void *privdata, void *obj);
};


struct dictStruct {
    // 类型特定函数
    dictType *type;
    
    // 私有数据
    void *privdata;
    
    // 哈希表  一般情况下,字典只使用 ht[0] 哈希表， ht[1] 哈希表只会在对 ht[0] 哈希表进行 rehash 时使用
    dictht ht[2];
    
    // rehash 索引
    // 当 rehash 不在进行时，值为 -1
    int rehashidx; /* rehashing not in progress if rehashidx == -1 */
};

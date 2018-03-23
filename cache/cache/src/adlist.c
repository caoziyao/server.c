//
//  adlist.c
//  cache
//
//  Created by working on 2018/3/24.
//  Copyright © 2018年 working. All rights reserved.
//  Redis 的双端链表实现

#include "adlist.h"

struct listNodeStruct {
    // 前置节点
    struct listNode *prev;
    
    // 后置节点
    struct listNode *next;
    
    // 节点的值
    void *value;
};

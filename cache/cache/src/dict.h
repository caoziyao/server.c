//
//  dict.h
//  cache
//
//  Created by working on 2018/3/25.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef dict_h
#define dict_h

#include <stdio.h>
#include <stdlib.h>


#include <stdio.h>
#include "adlist.h"


struct GwHashTableStruct;
typedef struct GwHashTableStruct GwHashTable;

// 创建并返回一个 hashtable
GwHashTable *
GwHashTableCreate(void);

// 往 hashtbale 中设置一个值, GwHashTable 只支持 int 类型的值
void
GwHashTableSet(GwHashTable *table, const char *key, int value);

// 检查 hashtable 中是否存在这个 key
bool
GwHashTableHas(GwHashTable *table, const char *key);

// 返回 hashtable 中 key 对应的值, 不考虑 key 不存在的情况, 用户应该用 GwHashTableHas 自行检查是否存在
int
GwHashTableGet(GwHashTable *table, const char *key);

// 销毁一个 hashtable
void
GwHashTableRemove(GwHashTable *table);

/*
 GwHashTable log
 */
void
GwHashTableLog(GwHashTable *table);

/*
 测试 2 个 hash table 是否相等
 */
bool
equalGwHashTable(GwHashTable *src, GwHashTable *dest);

#endif /* dict_h */

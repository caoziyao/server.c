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


struct dictStruct;
typedef struct dictStruct dict;

// 创建并返回一个 hashtable
dict *
dictCreate(void);

// 往 hashtbale 中设置一个值, dict 只支持 int 类型的值
void
dictSet(dict *table, const char *key, int value);

// 检查 hashtable 中是否存在这个 key
bool
dictHas(dict *table, const char *key);

// 返回 hashtable 中 key 对应的值, 不考虑 key 不存在的情况, 用户应该用 dictHas 自行检查是否存在
int
dictGet(dict *table, const char *key);

// 销毁一个 hashtable
void
dictRemove(dict *table);

/*
 dict log
 */
void
dictLog(dict *table);

/*
 测试 2 个 hash table 是否相等
 */
bool
equaldict(dict *src, dict *dest);

#endif /* dict_h */

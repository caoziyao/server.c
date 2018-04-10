//
//  dict.c
//  cache
//
//  Created by working on 2018/3/25.
//  Copyright © 2018年 working. All rights reserved.
//

#include "dict.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dict.h"
#define MaxSize 1000    // 暂时指定 1000 个长度

// 结构的具体定义
struct dictStruct {
    GwList *list[MaxSize];
};


// 创建并返回一个 hashtable
dict *
dictCreate(){
    
    // malloc 申请一块内存, 并初始化一下
    dict *t = malloc(sizeof(dict));
    
    for (int i = 0; i < MaxSize; i++) {
        GwList *l = GwListCreate(NULL, 0);
        t->list[i] = l;
    }
    
    return t;
}


int
_LengthOfString(const char *s) {
    
    int len = 0;
    const char *n = s;
    while(*n++ != '\0') {
        len++;
    }
    
    return len;
}


int
_GwHashOfKey(const char *key) {
    int n = 1;
    int f = 1;
    
    while(*key != '\0') {
        n += (int)(*key) * f;
        f += 10;
        key++;
    }
    
    return n;
}


/*
 先计算出下标
 */
int
_GwIndexOfKey(const char *key) {
    return _GwHashOfKey(key) % MaxSize;
}


/*
 insert at index
 */
void
_GwInsertAtIndex(dict *table, int index, const char *key, int value) {
    
    GwList *l = table->list[index];
    
    GwListAppendKeyValue(l, key, value);
    
}


// 往 hashtbale 中设置一个值, dict 只支持 int 类型的值
void
dictSet(dict *table, const char *key, int value){
    
    int index = _GwIndexOfKey(key);
    _GwInsertAtIndex(table, index, key, value);
}


// 检查 hashtable 中是否存在这个 key
bool
dictHas(dict *table, const char *key){
    
    int index = _GwIndexOfKey(key);
    GwList *l = table->list[index];
    if (GwListHasKey(l, key)) {
        return true;
    }
    
    return false;
}


// 返回 hashtable 中 key 对应的值, 不考虑 key 不存在的情况, 用户应该用 dictHas 自行检查是否存在
int
dictGet(dict *table, const char *key){
    int index = _GwIndexOfKey(key);
    GwList *l = table->list[index];
    
    return GwListRetValue(l, key);
}


// 删除一个 key
void
dictDel(dict *table, const char *key) {
    int index = _GwIndexOfKey(key);
    GwList *l = table->list[index];
    
    GwListDelElement(l, key);
}


// 销毁一个 hashtable
void
dictRemove(dict *table){
    
    for (int i = 0; i < MaxSize; i++) {
        GwList *l = table->list[i];
        GwListRemove(l);
    }
    free(table);
}


/*
 dict log
 */
void
dictLog(dict *table) {
    
    for (int i = 0; i < MaxSize; i++) {
        GwList *l = table->list[i];
        if (!GwListIsEmpty(l)) {
            printf("%d: ", i);
            GwListLog(table->list[i]);
        }
    }
}


/*
 测试 2 个 hash table 是否相等
 */
bool
equaldict(dict *src, dict *dest) {
    
    for (int i = 0; i < MaxSize; i++) {
        GwList *l1 = src->list[i];
        GwList *l2 = dest->list[i];
        if(!equalGwListKeyValue(l1, l2)) {
            return false;
        }
    }
    return true;
}


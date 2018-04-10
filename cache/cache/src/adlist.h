//
//  adlist.h
//  cache
//
//  Created by working on 2018/3/24.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef adlist_h
#define adlist_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// interface
// 声明 结构名, 类型
struct GwNodeStruct;
typedef struct GwNodeStruct GwNode;
struct GwListStruct;
typedef struct GwListStruct GwList;
typedef int type;


GwList *
GwListCreate(int *element, int numberOfElements);

void
GwListLog(GwList *list);


/*
 插入 hash table
 */
void
GwListAppendKeyValue(GwList *list, const char *key, type value);

/*
 返回一个 GwList 的长度
 实现 O(1) 时间复杂度的 GwListLength, 可以更改 GwList 的内部实现
 */
int
GwListLength(GwList *list);

/*
 检查一个 GwList 中是否存在某个元素
 */
bool
GwListContains(GwList *list, type element);


/*
 在 GwList 的末尾添加一个元素
 */
void
GwListAppend(GwList *list, type element);

/*
 在 GwList 的头部添加一个元素
 */
void
GwListPrepend(GwList *list, type element);

/*
 在一个 GwList 中查找某个元素, 返回下标(序号)
 如果不存在, 返回 -1
 */
int
GwListIndexOfElement(GwList *list, type element);

/*
 往一个 GwList 中插入一个元素, 下标(序号) 为 index
 不考虑非法情况(下标大于长度)
 */
void
GwListInsertElementAtIndex(GwList *list, type element, int index);

/*
 判断 list 是否相等
 返回 bool
 */
bool
equalGwList(GwList *src, GwList *dest);


/*
 判断 list 是否为空
 */
bool
GwListIsEmpty(GwList *list);


/*
 返回第一个元素
 */
type
GwListFirstElement(GwList *list);


/*
 移除第一个元素
 */
void
GwListRemoveFirstElement(GwList *list);


// 删除一个元素
void
GwListDelElement(GwList *list, const char *key);

/*
 复制字符串
 */
char *
GwStrCpy(char *dest, const char *src);

/*
 比较字符串
 */
bool
GwStrCmp(const char *s1,const char *s2);


/*
 判断 node key 是否相等
 返回 bool
 */
bool
GwListHasKey(GwList *list, const char *key);


/*
 判断 node key 是否相等并返回 value 值
 返回 value
 */
type
GwListRetValue(GwList *list, const char *key);

/*
 判断 list key value是否相等
 返回 bool
 */
bool
equalGwListKeyValue(GwList *src, GwList *dest);


/*
 删除 list 中所有元素并销毁
 */
void
GwListRemove(GwList *list);


#endif /* adlist_h */

//
//  gwlist.h
//  server
//
//  Created by working on 2018/3/20.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwlist_h
#define gwlist_h

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// interface
// 声明 结构名, 类型
typedef struct GwNodeStruct GwNode;
typedef struct GwListStruct GwList;
typedef int type;

GwList *
GwListCreate(int *element, int numberOfElements);

// 返回一个 GwList 的长度
int
GwListLength(GwList *list);

// 检查一个 GwList 中是否存在某个元素
bool
GwListContains(GwList *list, type element);

//  在 GwList 的末尾添加一个元素
void
GwListAppend(GwList *list, type element);

//  在 GwList 的头部添加一个元素
void
GwListPrepend(GwList *list, type element);

// 在一个 GwList 中查找某个元素, 返回下标(序号)
// 如果不存在, 返回 -1
int
GwListIndexOfElement(GwList *list, type element);

//往一个 GwList 中插入一个元素, 下标(序号) 为 index
//不考虑非法情况(下标大于长度)
void
GwListInsertElementAtIndex(GwList *list, type element, int index);

// 判断 list 是否为空
bool
GwListIsEmpty(GwList *list);

// 返回第一个元素
type
GwListFirstElement(GwList *list);

// 移除第一个元素
void
GwListRemoveFirstElement(GwList *list);

void
GwListLog(GwList *list);

#endif /* gwlist_h */

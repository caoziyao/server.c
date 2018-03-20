//
//  gwlist.c
//  server
//
//  Created by working on 2018/3/20.
//  Copyright © 2018年 working. All rights reserved.
//  todo 自定义链表

#include "gwlist.h"

// 结构的具体定义
struct GwNodeStruct {
    type    element;
    GwNode  *next;
};


struct GwListStruct{
    int     length;
    GwNode  *tail;
    GwNode  *next;
};


GwList *
GwListCreate(int *element, int numberOfElements) {
    int n = numberOfElements;
//    assert(n >= 0);
    
    GwList *l = malloc(sizeof(GwList));
    l->length = n;
    l->next = NULL;
    
    // 循环插入初始化元素
    for (int i = n - 1; i >= 0; i--) {
        GwNode *node = malloc(sizeof(GwNode));
        node->element = element[i];
        node->next = l->next;
    }
    return l;
}

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

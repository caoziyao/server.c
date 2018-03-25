//
//  adlist.c
//  cache
//
//  Created by working on 2018/3/24.
//  Copyright © 2018年 working. All rights reserved.
//  Redis 的双端链表实现

#include "adlist.h"

// 结构的具体定义
struct GwNodeStruct {
    type element;
    GwNode *next;
    char key[20];   // hash table key
    type value; // hash table value
};

struct GwListStruct{
    int length;
    GwNode *tail;
    GwNode *next;
};


// 创建并返回一个 List
// element 是一个 int 数组
// numberOfElements 是数组的长度
// 在 C 语言中, 数组的长度信息要额外提供
GwList *
GwListCreate(int *element, int numberOfElements) {
    // assert 是用于确保一定条件的断言
    assert(numberOfElements >= 0);
    
    // malloc 申请一块内存, 并初始化一下
    GwList *list = malloc(sizeof(GwList));
    list->length = numberOfElements;
    list->next = NULL;
    
    // 循环插入初始化元素
    for (int i = numberOfElements -1; i >= 0; i--) {
        GwNode *n = malloc(sizeof(GwNode));
        n->element = element[i];
        n->next = list->next;
        
        list->next = n;
        
        // GwList 长度
        if (i == numberOfElements - 1) {
            list->tail = n;
        }
        
    }
    
    return list;
}


// 把一个 List 的数据打印出来
void
GwListLog(GwList *list) {
    GwNode *l = list->next;
    while(l != NULL) {
        printf("e: %d key: %s value: %d\n", l->element, l->key, l->value);
        l = l->next;
    }
    printf("\n");
}


/*
 判断 list 是否为空
 */
bool
GwListIsEmpty(GwList *list) {
    return GwListLength(list) == 0;
}


/*
 返回一个 GwList 的长度
 实现 O(1) 时间复杂度的 GwListLength, 可以更改 GwList 的内部实现
 */
int
GwListLength(GwList *list) {
    
    return list->length;
}

/*
 检查一个 GwList 中是否存在某个元素
 */
bool
GwListContains(GwList *list, type element) {
    
    GwNode *l = list->next;
    while(l != NULL) {
        if (l->element == element) {
            return true;
        }
        l = l->next;
    }
    
    return false;
}

/*
 在 GwList 的末尾添加一个元素
 实现 O(1) 时间复杂度的 GwListAppend, 可以更改 GwList 的内部实现
 */
void
GwListAppend(GwList *list, type element) {
    
    GwNode *n = malloc(sizeof(GwNode));
    n->element = element;
    n->next = NULL;
    
    // 设置尾巴
    if (GwListIsEmpty(list)) {
        list->tail = n;
        list->next = n;
    } else {
        list->tail->next = n;
        list->tail = n;
    }
    
    list->length++;
}

/*
 复制字符串
 */
char *
GwStrCpy(char *dest, const char *src) {
    
    assert((dest != NULL) && (src != NULL));
    
    char *address = dest;
    
    while((*dest++ = *src++) != '\0');
    //    while(*src != '\0') {
    //        *dest++ = *src++;
    //    }
    //    *dest = '\0';
    return address;
}

/*
 比较字符串
 相同返回 true，不同返回 false
 */
bool
GwStrCmp(const char *s1,const char *s2){
    assert((s1 != NULL) && (s2 != NULL));
    
    while((*s1 != '\0') && (*s2 != '\0')) {
        if (*s1++ != *s2++) {
            return false;
        }
    }
    
    if ((*s1 == '\0') && (*s2 != '\0')) {
        return false;
    }
    
    if ((*s1 != '\0') && (*s2 == '\0')) {
        return false;
    }
    
    return true;
}


/*
 判断 node key 是否相等
 返回 bool
 */
bool
GwListHasKey(GwList *list, const char *key) {
    // 条件成立
    
    GwNode *n = list->next;
    while (n != NULL) {
        if (GwStrCmp(n->key, key) ) {
            return true;
        }
        n = n->next;
    }
    return false;
}

/*
 判断 node key 是否相等并返回 value 值
 返回 value
 */
type
GwListRetValue(GwList *list, const char *key) {
    
    GwNode *n = list->next;
    while (n != NULL) {
        if (GwStrCmp(n->key, key) ) {
            return n->value;
        }
        n = n->next;
    }
    
    return -1;
}

/*
 
 */
void
GwListAppendKeyValue(GwList *list, const char *key, type value) {
    GwNode *n = malloc(sizeof(GwNode));
    GwStrCpy(n->key, key);
    n->value = value;
    n->next = NULL;
    
    // 设置尾巴
    if (GwListIsEmpty(list)) {
        list->tail = n;
        list->next = n;
    } else {
        list->tail->next = n;
        list->tail = n;
    }
    
    list->length++;
}

/*
 1.4
 在 GwList 的头部添加一个元素
 */
void
GwListPrepend(GwList *list, type element) {
    
    // 新节点
    GwNode *n = malloc(sizeof(GwNode));
    n->element = element;
    
    n->next = list->next;
    list->next = n;
    
    list->length++;
    
}

/*
 1.5
 在一个 GwList 中查找某个元素, 返回下标(序号)
 如果不存在, 返回 -1
 */
int
GwListIndexOfElement(GwList *list, type element) {
    
    int index = -1;
    int i = 0;
    GwNode *l = list->next;
    while(l != NULL) {
        if (l->element == element) {
            index = i;
            break;
        }
        i++;
        l = l->next;
    }
    return index;
}


/*
 1.6
 往一个 GwList 中插入一个元素, 下标(序号) 为 index
 不考虑非法情况(下标大于长度)
 */
void
GwListInsertElementAtIndex(GwList *list, type element, int index) {
    assert(index >= 0);
    
    // 不考虑非法情况(下标大于长度)
    int len = GwListLength(list);
    assert(len >= index);
    
    
    if (index == 0) {
        GwListPrepend(list, element);
    } else {
        GwNode *l = list->next;
        
        int i = 1;
        while(i < index) {
            l = l->next;
            i++;
        }
        
        // 新节点
        GwNode *n = malloc(sizeof(GwNode));
        n->element = element;
        
        n->next = l->next;
        l->next = n;
        list->length++;
    }
}


/*
 判断 list 是否相等
 返回 bool
 */
bool
equalGwList(GwList *src, GwList *dest) {
    // 条件成立
    
    int lenSrc = GwListLength(src);
    int lenDest = GwListLength(dest);
    
    if (lenSrc != lenDest) {
        return false;
    }
    
    GwNode *l1 = src->next;
    GwNode *l2 = dest->next;
    while (l1 != NULL){
        if (l1->element != l2->element) {
            return false;
        }
        l1 = l1->next;
        l2 = l2->next;
    }
    return true;
}

/*
 判断 list key value是否相等
 返回 bool
 */
bool
equalGwListKeyValue(GwList *src, GwList *dest) {
    
    // 条件成立
    int lenSrc = GwListLength(src);
    int lenDest = GwListLength(dest);
    
    if (lenSrc != lenDest) {
        return false;
    }
    
    GwNode *l1 = src->next;
    GwNode *l2 = dest->next;
    while (l1 != NULL){
        if ((!GwStrCmp(l1->key, l2->key)) || (l1->value != l2->value)) {
            return false;
        }
        l1 = l1->next;
        l2 = l2->next;
    }
    return true;
}


/*
 返回第一个元素
 */
type
GwListFirstElement(GwList *list) {
    
    if (!GwListIsEmpty(list)) {
        return list->next->element;
    } else {
        return 0;
    }
    
}


/*
 移除第一个元素
 */
void
GwListRemoveFirstElement(GwList *list) {
    list->length--;
    //
    GwNode *n = list->next;
    list->next = n->next;
    free(n);
}


/*
 删除 list 中所有元素并销毁
 */
void
GwListRemove(GwList *list) {
    
    while(list->length != 0) {
        GwListRemoveFirstElement(list);
    }
    free(list);
}






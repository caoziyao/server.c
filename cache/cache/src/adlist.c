//
//  adlist.c
//  cache
//
//  Created by working on 2018/3/24.
//  Copyright © 2018年 working. All rights reserved.
//  Redis 的双端链表实现

/*
 双端： 链表节点带有 prev 和 next 指针， 获取某个节点的前置节点和后置节点的复杂度都是 O(1) 。
 无环： 表头节点的 prev 指针和表尾节点的 next 指针都指向 NULL ， 对链表的访问以 NULL 为终点。
 带表头指针和表尾指针： 通过 list 结构的 head 指针和 tail 指针， 程序获取链表的表头节点和表尾节点的复杂度为 O(1) 。
 带链表长度计数器： 程序使用 list 结构的 len 属性来对 list 持有的链表节点进行计数， 程序获取链表中节点数量的复杂度为 O(1) 。
 多态： 链表节点使用 void* 指针来保存节点值， 并且可以通过 list 结构的 dup 、 free 、 match 三个属性为节点值设置类型特定函数， 所以链表可以用于保存各种不同类型的值。
 */

#include "adlist.h"

struct listNodeStruct {
    // 前置节点
    struct listNode *prev;
    
    // 后置节点
    struct listNode *next;
    
    // 节点的值
    void *value;
};


struct listStruct {
    // 表头节点
    listNode *head;
    
    // 表尾节点
    listNode *tail;
    
    // 链表所包含的节点数量
    unsigned long len;
    
    // 节点值复制函数
    void *(*dup)(void *ptr);
    
    // 节点值释放函数
    void (*free)(void *ptr);
    
    // 节点值对比函数
    int (*match)(void *ptr, void *key);
};


/*
 * 创建一个新的链表
 *
 * 创建成功返回链表，失败返回 NULL 。
 *
 * T = O(1)
 */
list *
list_create(void) {
    list *l;
    
    l = malloc(sizeof(list));
    if (l == NULL) {
        printf("list_create()\n");
        return NULL;
    }
    
    // 初始化属性
    l->head = l->tail = NULL;
    l->len = 0;
    l->dup = NULL;
    l->free = NULL;
    l->match = NULL;
    
    return l;
}


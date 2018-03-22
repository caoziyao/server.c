//
//  zmalloc.c
//  cache
//
//  Created by working on 2018/3/22.
//  Copyright © 2018年 working. All rights reserved.
//  todo 字节对齐

#include "zmalloc.h"

// 64位 一个字长(8个字节)
#define PREFIX_SIZE sizeof(size_t)

void *
zmalloc(size_t size) {
    void *ptr = malloc(size + PREFIX_SIZE);
    return NULL;
}

void *
zcalloc(size_t size) {
    return NULL;
}

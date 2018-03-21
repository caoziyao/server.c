//
//  zmalloc.h
//  cache
//
//  Created by working on 2018/3/22.
//  Copyright © 2018年 working. All rights reserved.
//  zmalloc - total amount of allocated memory aware version of malloc()

#ifndef zmalloc_h
#define zmalloc_h

#include <stdio.h>
#include <stdlib.h>

void *
zmalloc(size_t size);

void *
zcalloc(size_t size);

#endif /* zmalloc_h */

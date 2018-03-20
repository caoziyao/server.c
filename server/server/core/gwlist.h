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

void
GwListLog(GwList *list);

#endif /* gwlist_h */

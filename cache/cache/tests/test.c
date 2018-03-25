//
//  test.c
//  cache
//
//  Created by working on 2018/3/25.
//  Copyright © 2018年 working. All rights reserved.
//

#include "test.h"
#include "sds.h"
#include "zmalloc.h"
#include "adlist.h"
#include "dict.h"

void
ensure(bool condition, const char *message) {
    // 条件成立
    if(!condition) {
        printf("测试失败: %s\n", message);
    }
}


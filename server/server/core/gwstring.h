//
//  gwstring.h
//  server
//
//  Created by working on 2018/3/19.
//  Copyright © 2018年 working. All rights reserved.
//

#ifndef gwstring_h
#define gwstring_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct _GwString GwString;

// 初始化字符串
GwString *
GwStringStr(char *str);

// 初始化字符串为空字符串，符串的长度为0，data为NULL
GwString *
GwStringNullStr(char *str);

// 字符串str为text，由于使用sizeof计算长度，故text必须为常量字符串
GwString *
GwStringSet(GwString *str, char *text);

// 区分大小写的字符串比较，只比较前n个字符。
GwString *
GwStringnCmp(GwString *s1, GwString *s2, int n);

#endif /* gwstring_h */

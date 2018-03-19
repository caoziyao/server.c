//
//  gwstring.c
//  server
//
//  Created by working on 2018/3/19.
//  Copyright © 2018年 working. All rights reserved.
//

// todo string 数据结构
// data指向字符串数据的第一个字符，字符串的结束用长度来表示，而不是由’\0’来表示结束
#include "gwstring.h"


//typedef    unsigned char     u_char;
struct _GwString {
    size_t len;
    char *data;
};


// #define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
GwString *
GwStringStr(char *str) {
    GwString *s = malloc(sizeof(GwString));
    *s = (GwString) {
//        sizeof(str) - 1,  // 可能出错
        strlen(str),
        (char *) str
    };
    return s;
}


// 初始化字符串为空字符串，符串的长度为0，data为NULL
GwString *
GwStringNullStr(char *str) {
    return NULL;
}


// 字符串str为text，由于使用sizeof计算长度，故text必须为常量字符串
GwString *
GwStringSet(GwString *str, char *text) {
    return NULL;
}


// 区分大小写的字符串比较，只比较前n个字符。
GwString *
GwStringnCmp(GwString *s1, GwString *s2, int n) {
    return NULL;
}


// test
void
GwStringTest() {
    char *str = "hello aaa";
    GwString *hllo = GwStringStr(str);

    printf("sssss\n");
    free(hllo);
}

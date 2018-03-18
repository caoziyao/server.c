//
//  gwstring.c
//  server
//
//  Created by working on 2018/3/19.
//  Copyright © 2018年 working. All rights reserved.
//

// todo string 数据结构
//data指向字符串数据的第一个字符，字符串的结束用长度来表示，而不是由’\0’来表示结束
#include "gwstring.h"


//typedef    unsigned char     u_char;
struct _GwString {
    size_t len;
    u_char *data;
};


// #define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
GwString *
GwStringStr(char *str) {
    GwString *s = malloc(sizeof(GwString));
    *s = (GwString) {
        sizeof(str) - 1,
        (u_char *) str
    };
    return s;
}


// test
void
GwStringTest() {
    char *str = "hello aaa";
    GwString *hllo = GwStringStr(str);

    printf("sssss\n");
    free(hllo);
}

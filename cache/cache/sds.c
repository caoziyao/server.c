//
//  sds.c
//  cache
//
//  Created by working on 2018/3/22.
//  Copyright © 2018年 working. All rights reserved.

//  动态字符串实现 simple dynamic string，

#include "sds.h"

struct _sds_hdr_struct {
    // 记录 buf 数组中已使用字节的数量
    // 等于 SDS 所保存字符串的长度
    int len;
    
    // 记录 buf 数组中未使用字节的数量
    int free;
    
    // 字节数组，用于保存字符串
    char buf[];
};


/*
 * 根据给定的初始化字符串 init 和字符串长度 initlen
 * 创建一个新的 sds
 *
 * 参数
 *  init ：初始化字符串指针
 *  initlen ：初始化字符串的长度
 *
 * 返回值
 *  sds ：创建成功返回 sdshdr 相对应的 sds
 *        创建失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds
sds_newlen(const void *init, size_t initlen) {
    sds_hdr *sh;
    
    // 根据是否有初始化内容，选择适当的内存分配方式
    if (init != NULL) {
        // zmalloc 不初始化所分配的内存
        sh = mall
    } else {
        
    }
}

/*
 * 根据给定字符串 init ，创建一个包含同样字符串的 sds
 *
 * 参数
 *  init ：如果输入为 NULL ，那么创建一个空白 sds
 *         否则，新创建的 sds 中包含和 init 内容相同字符串
 *
 * 返回值
 *  sds ：创建成功返回 sdshdr 相对应的 sds
 *        创建失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds
sds_new(const char *init) {
    size_t len = (init == NULL) ? 0 : strlen(init);
    return sds_newlen(init, initlen);
}

// 创建一个不包含任何内容的空 SDS 。
// O(1)
sds
sds_empty(void);

// 释放给定的 SDS 。
// O(1)
void
sds_free(sds s);

// 返回 SDS 的已使用空间字节数
// O(1) 。
size_t
sds_len(const sds s);

// 返回 SDS 的未使用空间字节数。O(1) 。
size_t
sds_avail(const sds s);

// 创建一个给定 SDS 的副本（copy）。
// O(N)，N 为给定 SDS 的长度。
sds
sds_dup(const sds s);

// 清空 SDS 保存的字符串内容。因为惰性空间释放策略，复杂度为
// O(1) 。
void
sds_clear(sds s);

// 将给定 C 字符串拼接到 SDS 字符串的末尾。
// O(N), N 为被拼接 C 字符串的长度。
sds
sds_cat(sds s, const char *t);

// 将给定 SDS 字符串拼接到另一个 SDS 字符串的末尾。
// O(N), N 为被拼接 SDS 字符串的长度。
sds
sds_catsds(sds s, const sds t);

// 将给定的 C 字符串复制到 SDS 里面， 覆盖 SDS 原有的字符串。
// O(N), N为被复制 C 字符串的长度。
sds
sds_cpy(sds s, const char *t);

// 用空字符将 SDS 扩展至给定长度。
// O(N), N 为扩展新增的字节数。
sds
sds_growzero(sds s, size_t len);

// 保留 SDS 给定区间内的数据， 不在区间内的数据会被覆盖或清除。
// O(N), N 为被保留数据的字节数。
void
sds_range(sds s, int start, int end);

// 接受一个 SDS 和一个 C 字符串作为参数， 从 SDS 左右两端分别移除所有在 C 字符串中出现过的字符。
// O(M*N) ， M 为 SDS 的长度， N 为给定 C 字符串的长度。
sds
sds_trim(sds s, const char *cset);

// 对比两个 SDS 字符串是否相同。
// O(N) ， N 为两个 SDS 中较短的那个 SDS 的长度。
int
sds_cmp(const sds s1, const sds s2);





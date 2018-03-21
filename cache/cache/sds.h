//
//  sds.h
//  cache
//
//  Created by working on 2018/3/22.
//  Copyright © 2018年 working. All rights reserved.
//  SDS 的主要操作 API

#ifndef sds_h
#define sds_h

#include <stdio.h>
#include <sys/types.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

// 类型别名，用于指向 sdshdr 的 buf 属性
typedef char *sds;

// 保存字符串对象的结构
typedef struct _sds_hdr_struct sds_hdr;

// 创建一个包含给定 C 字符串的 SDS 。
// O(N)， N 为给定 C 字符串的长度
sds
sds_new(const char *init);

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


#endif /* sds_h */

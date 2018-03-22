//
//  sds.c
//  cache
//
//  Created by working on 2018/3/22.
//  Copyright © 2018年 working. All rights reserved.

//  动态字符串实现 simple dynamic string，

#include "sds.h"
#define SDS_MAX_PREALLOC (1024 * 1024)

struct _sds_hdr_struct {
    // 记录 buf 数组中已使用字节的数量
    // 等于 SDS 所保存字符串的长度
    size_t len;
    
    // 记录 buf 数组中未使用字节的数量
    size_t free;
    
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
sds_hdr *
sds_newlen(const void *init, size_t initlen) {
    sds_hdr *sh;
    size_t len = initlen;
    
    // 根据是否有初始化内容，选择适当的内存分配方式
    sh = malloc(sizeof(sds_hdr) + len + 1);
    // 将分配的内存全部初始化为 0
    memset((void *)sh, 0, sizeof(sh));
    if (sh == NULL) {
        return NULL;
    }
    
    sh->len = len;
    sh->free = 0;
    
    if (len && init) {
        memcpy(sh->buf, init, len);
    }
    // 以 \0 结尾
    sh->buf[len] = '\0';
    
    // 返回整个 sdshdr
    return sh;
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
sds_hdr *
sds_new(const char *init) {
    size_t len = (init == NULL) ? 0 : strlen(init);
    return sds_newlen(init, len);
}

/*
 * 创建并返回一个只保存了空字符串 "" 的 sds
 *
 * 返回值
 *  sds ：创建成功返回 sdshdr 相对应的 sds
 *        创建失败返回 NULL
 *
 * 复杂度
 *  T = O(1)
 */
sds_hdr *
sds_empty(void) {
    return sds_newlen("", 0);
}


/*
 * 释放给定的 sds
 *
 * 复杂度
 *  T = O(N)
 */
void
sds_free(sds_hdr *sh) {
    if (sh == NULL) {
        return;
    }
    free(sh);
}


/*
 * 返回 sds 实际保存的字符串的长度
 *
 * T = O(1)
 */
size_t
sds_len(sds_hdr *sh) {
    if (sh == NULL) {
        return 0;
    }
    return sh->len;
}


/*
 * 返回 sds 可用空间的长度
 *
 * T = O(1)
 */
size_t
sds_avail(sds_hdr *sh) {
    if (sh == NULL) {
        return 0;
    }
    return sh->free;
}


/*
 * 复制给定 sds 的副本
 *
 * 返回值
 *  sds ：创建成功返回输入 sds 的副本
 *        创建失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds_hdr *
sds_dup(sds_hdr *sh) {
    sds s = sh->buf;
    size_t len = sh->len;
    return sds_newlen(s, len);
}


/*
 * 在不释放 SDS 的字符串空间的情况下，
 * 重置 SDS 所保存的字符串为空字符串。
 *
 * 复杂度
 *  T = O(1)
 */
void
sds_clear(sds_hdr *sh) {
    // 取出 sdshdr
//    struct sdshdr *sh = (void*) (s-(sizeof(struct sdshdr)));
    
    // 重新计算属性
    sh->free += sh->len;
    sh->len = 0;
    
    // 将结束符放到最前面（相当于惰性地删除 buf 中的内容）
    sh->buf[0] = '\0';
}


/*
 * 对 sds 中 buf 的长度进行扩展，确保在函数执行之后，
 * buf 至少会有 addlen + 1 长度的空余空间
 * （额外的 1 字节是为 \0 准备的）
 *
 * 返回值
 *  sds ：扩展成功返回扩展后的 sds
 *        扩展失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds_hdr *
sdsMakeRoomFor(sds_hdr *sh, size_t addlen) {
    sds_hdr *newsh;
    size_t  len, newlen;
    
    // 获取 s 目前的空余空间长度
    size_t len_free = sds_avail(sh);
    
    // s 目前的空余空间已经足够，无须再进行扩展，直接返回
    if (len_free >= addlen) {
        return sh;
    }
    
    // 获取 sh 目前已占用空间的长度
    len = sds_len(sh);
    
    // s 最少需要的长度
    newlen = len + addlen;
    
    // 根据新长度，为 s 分配新空间所需的大小
    if (newlen < SDS_MAX_PREALLOC) {
        // 如果新长度小于 SDS_MAX_PREALLOC
        // 那么为它分配两倍于所需长度的空间
        newlen *= 2;
    } else {
        // 否则，分配长度为目前长度加上 SDS_MAX_PREALLOC
        newlen += SDS_MAX_PREALLOC;
    }
    // T = O(N)
    newsh = malloc(newlen+1);
    if (newsh == NULL) {
        return NULL;
    }
    
    // 更新 sds 的空余长度
    newsh->free = newlen - len;
    
    return newsh;
}


// 返回 buf
sds
sds_char(sds_hdr *sh) {
    return sh->buf;
}


/*
 * 将长度为 len 的字符串 t 追加到 sds 的字符串末尾
 *
 * 返回值
 *  sds ：追加成功返回新 sds ，失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds_hdr *
sds_catlen(sds_hdr *sh, const void *t, size_t len) {
    sds_hdr *new_sh;
    
    // 原有字符串长度
    size_t curlen = sds_len(sh);
    
    // 扩展 sds 空间
    // T = O(N)
    sh = sdsMakeRoomFor(sh,len);
    if (sh == NULL) {
        return NULL;
    }
    
    // 复制 t 中的内容到字符串后部
    memcpy(sh->buf + curlen, t, len);
    
    // 更新属性
    sh->len = curlen + len;
    sh->free = sh->free - len;
    
    // 添加新结尾符号
    sh->buf[curlen+len] = '\0';
    
    // 返回新 sds_hdr
    return sh;
}


/*
 * 将给定字符串 t 追加到 sds 的末尾
 *
 * 返回值
 *  sds ：追加成功返回新 sds ，失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds_hdr *
sds_cat(sds_hdr *sh, const char *t) {
    return sds_catlen(sh, t, strlen(t));
}


/*
 * 将另一个 sds 追加到一个 sds 的末尾
 *
 * 返回值
 *  sds ：追加成功返回新 sds ，失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds_hdr *
sds_catsds(sds_hdr *sh, sds_hdr *th) {
    return sds_catlen(sh, sds_char(th), sds_len(th));
}


/*
 * 将字符串 t 的前 len 个字符复制到 sds s 当中，
 * 并在字符串的最后添加终结符。
 *
 * 如果 sds 的长度少于 len 个字符，那么扩展 sds
 *
 * 复杂度
 *  T = O(N)
 *
 * 返回值
 *  sds ：复制成功返回新的 sds ，否则返回 NULL
 */
sds_hdr *
sds_cpylen(sds_hdr *sh, const char *t, size_t len) {
    // sds 现有 buf 的长度
    size_t totlen = sh->free + sh->len;
    
    // 如果 s 的 buf 长度不满足 len ，那么扩展它
    if (totlen < len) {
        // T = O(N)
        sh = sdsMakeRoomFor(sh, len - sh->len);
        if (sh == NULL) {
            return NULL;
        }
        totlen = sh->free + sh->len;
    }
    
    // 复制内容
    // T = O(N)
    memcpy(sh->buf, t, len);
    
    // 添加终结符号
    sh->buf[len] = '\0';
    
    // 更新属性
    sh->len = len;
    sh->free = totlen-len;
    
    return sh;
}


/*
 * 将字符串复制到 sds 当中，
 * 覆盖原有的字符。
 *
 * 如果 sds 的长度少于字符串的长度，那么扩展 sds 。
 *
 * 复杂度
 *  T = O(N)
 *
 * 返回值
 *  sds ：复制成功返回新的 sds ，否则返回 NULL
 */
sds_hdr *
sds_cpy(sds_hdr *sh, const char *t) {
    return sds_cpylen(sh, t, strlen(t));
}

// 用空字符将 SDS 扩展至给定长度。
// O(N), N 为扩展新增的字节数。
sds_hdr *
sds_growzero(sds_hdr *sh, size_t len);

// 保留 SDS 给定区间内的数据， 不在区间内的数据会被覆盖或清除。
// O(N), N 为被保留数据的字节数。
void
sds_range(sds_hdr *sh, int start, int end);

// 接受一个 SDS 和一个 C 字符串作为参数， 从 SDS 左右两端分别移除所有在 C 字符串中出现过的字符。
// O(M*N) ， M 为 SDS 的长度， N 为给定 C 字符串的长度。
sds_hdr *
sds_trim(sds_hdr *sh, const char *cset);

// 对比两个 SDS 字符串是否相同。
// O(N) ， N 为两个 SDS 中较短的那个 SDS 的长度。
int
sds_cmp(sds_hdr *s1, sds_hdr * s2);





//
//  rdb.c
//  cache
//
//  Created by working on 2018/4/10.
//  Copyright © 2018年 working. All rights reserved.
//
/*
 rdb 文件结构
 REDIS db_version databases  EOF     check_sum
        4字节                1字节       8字节
 
 databases：
    SELECTDB  db_number   key_value_pairs
       1字节    数据库号码
 
 key_value_pairs:
    TYPE  key value
 
 */

#include "rdb.h"

// save 阻塞 redis 服务
void
rdbSave() {
    
}


// 派生子进程 save
void
rdbBgSave() {
    
}


// load
void
rdbLoad() {
    
}

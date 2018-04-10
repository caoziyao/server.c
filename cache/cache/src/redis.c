//
//  redis.c
//  cache
//
//  Created by working on 2018/4/10.
//  Copyright © 2018年 working. All rights reserved.
//

#include "redis.h"

# define DefaultDbNum 16    // 默认数据库数量

static redisServer *server;
static redisClient *client;

// 数据库
struct redisDbStruct{
    
    // 数据库键空间，保存数据库中的所有键值对
    dict *dict;
};


// server
struct redisServerStruct{
    
    // 服务器的数据数量
    int dbnum;
    
    // 一个数组，保存服务器中的所有数据库
    redisDb *db[DefaultDbNum];
    
    // 修改计数器
    long long dirty;
    // 上一次执行的时间, 时间戳
    time_t lastsave;
};


// client
struct redisClientStruct{
    
    // 记录客户端当前正在使用的数据库
    redisDb *db;
};


// 创建 db
redisDb *
_redisDbCreate() {
    dict *table = dictCreate();
    redisDb *db = malloc(sizeof(redisDb));
    db->dict = table;
    
    return db;
}


// 数据库初始化
redisServer *
redisServerInit() {
    redisServer *s = malloc(sizeof(redisServer));
    client = malloc(sizeof(redisClient));
    
    s->dbnum = DefaultDbNum;
    
    for (int i = 0; i < s->dbnum; i++) {
        redisDb *db = _redisDbCreate();
        s->db[i] = db;
    }
    
    client->db = s->db[0];
    
    server = s;
    return server;
}


// set
void
redisServerSet(const char *key, int value) {
    redisDb *db = client->db;
    dict *table = db->dict;
    
    dictSet(table, key, value);
}


// get
int
redisServerGet(const char *key) {
    redisDb *db = client->db;
    dict *table = db->dict;
    
    int value = dictGet(table, key);
    return value;
}


// del
void
redisServerDel(const char *key) {
    redisDb *db = client->db;
    dict *table = db->dict;
    
    dictDel(table, key);
}


// 切换数据库
void
redisServerSelect(int number) {
    int n = server->dbnum;
    
    if (number > n - 1) {
        printf("out of dbnum %d\n", n);
    } else {
        client->db = server->db[number];
    }
}


// 监听客户端



// test
void
redisServerTest() {
    char *key = "abc";
    redisServer *s = redisServerInit();
    
    redisServerSet(key, 123);
    int value = redisServerGet(key);
    
    printf("value %d\n", value);
    
    
    redisServerSelect(2);
    printf("value2 %d\n", redisServerGet(key));
    
    redisServerSet(key, 234);
    
    printf("value2 %d\n", redisServerGet(key));
    
    redisServerDel(key);
    printf("del %d\n", redisServerGet(key));
    
    redisServerSet(key, 444);
    
    printf("value2 %d\n", redisServerGet(key));
    
}

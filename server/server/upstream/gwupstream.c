//
//  gwupstream.c
//  server
//
//  Created by working on 2018/4/1.
//  Copyright © 2018年 working. All rights reserved.
/*  todo
 upstream模块主要做两件事情：
 1. 当外部的客户端发送一个http请求后，如果涉及更后台服务，则会创建一个到后端服务的request请求；
 2. 请求到达后端，然后处理完成后，则upstream会将返回的数据接收过来，然后发送给外部请求的客户端。
 
方案1， 轮询法
 轮询法,很好理解,将请求按照顺序轮流的分配到服务器上,他均衡的对待每一台后端的服务器,
 不关心服务器的的连接数和负载情况.以下代码演示了这种算法.
 
 方案2，随机法
 通过系统的随机函数,根据后端服务器列表的大小来随机获取其中的一台来访问,随着调用量的增大,
 实际效果越来越近似于平均分配到没一台服务器.和轮询的效果类似.
 
 方案3，源地址hash法.
 源地址hash法的思想是获取客户端访问的ip地址,通过hash函数计算出一个hash值,用该hash值对服
 务器列表的大小进行取模运算,得到的值就是要访问的服务器的序号.
 
 方案4，加权轮询法，加权随机法,
 刚刚有说道过,不同的服务器性能不同,所以不能一概而论,需要给性能低的服务器给比较低的
 权重,性能高的给跟高的权重.
 
 唯一性 hash
 简单的说，在移除 / 添加操作，它能够尽可能小的改变已存在 key 映射关系.
 */

#include "gwupstream.h"

// 定义 uppstream
struct _GwUpstream{
    int numberOfServer;
    int index;
    GwAddress *address[100];    // server
    int healthCheckDelay;       // 对同一台后端服务器两次检测之间的时间间隔，单位毫秒，默认为1000。
    int healthCheckTimeout;     // 进行一次健康检测的超时时间，单位为毫秒，默认值2000。
    int healthCheckFailCount;  // 对一台后端服务器检测成功或失败多少次之后方才确定其为成功或失败，并实现启用或禁用此服务器。
};


// 初始化
GwUpstream *
GwUpstreamInit() {
    GwPool *pool = GwPoolCreate(sizeof(GwUpstream) + sizeof(GwAddress) * 3);
    GwUpstream *up = (GwUpstream *)GwPoolPallc(pool, sizeof(GwUpstream));
    
    GwAddress *addr1 = (GwAddress *)GwPoolPallc(pool, sizeof(GwAddress));
    GwAddress *addr2 = (GwAddress *)GwPoolPallc(pool, sizeof(GwAddress));
    char *host = "127.0.0.1";
    addr1->host = host;
    addr1->port = 3000;
    
    addr2->host = host;
    addr2->port = 3005;
    
    up->address[0] = addr1;
    up->address[1] = addr2;
    up->numberOfServer = 2;
    up->index = 0;
    
    return up;
}


// 获得服务器实例
GwAddress *
GwUpstreamGetServer(GwUpstream *upstream) {
    GwUpstream *up = upstream;
    int n = up->numberOfServer;
    int index = up->index;
    
    int next = (index + 1) % n;
    
    GwAddress *addr = up->address[next];
    
    up->index = next;
    return addr;
}


//
void
GwUpstreamDestroy() {
//    GwPoolDestroy(pool);
}



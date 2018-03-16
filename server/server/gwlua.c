//
//  gwlua.c
//  server
//
//  Created by working on 2018/3/16.
//  Copyright © 2018年 working. All rights reserved.
//
/*
 gwlua 对 lua 封装
 */
#include "gwlua.h"
#include "utils.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

static lua_State *L;

void
GwLuaInitEnv(){
    
    // 创建 lua 运行环境
    L = luaL_newstate();
    // 加载 lua 标准库
    luaL_openlibs(L);
}


void
GwLuaCloseEnv(){
    lua_close(L);
}

// 加载文件
void *
GwLuaDoFile(char *filename) {
    // "server.lua"
    if(luaL_dofile(L, filename)) {
        printf("LUA ERROR: %s\n", lua_tostring(L, -1));
        return NULL;
    }
    return NULL;
}


const char *
GuwLuaExecl(char *filename, char *arg){
    if(luaL_dofile(L, filename)) {
        printf("LUA ERROR: %s\n", lua_tostring(L, -1));
        return "NULL";
    }
    int n1 = 1;
    int n2 = 2;
    
    lua_getglobal(L, "luaadd");
    lua_pushstring(L, arg);
    if(lua_pcall(L, 1, 1, 0) != 0) {
        printf("LUA ERROR: %s\n", lua_tostring(L, -1));
    }

    const char * n3 = lua_tostring(L, -1);
    lua_pop(L, 1);

    return n3;
}


const char *
responseFromLua(lua_State *L, char *request) {
    int n1 = 1;
    int n2 = 2;
    //    dump(L);
    // 因为 L 已经 dofile 了 gua.lua
    // 所以 L 这个环境中有 luaadd 这个变量
    // getglobal 获取到的变量会 push 到栈
    lua_getglobal(L, "luaadd");
    //    dump(L);
    // 用 lua_pushnumber 压 2 个数字入栈
    lua_pushstring(L, request);
    //    lua_pushnumber(L, n1);
    //    dump(L);
    //    lua_pushnumber(L, n2);
    //    dump(L);
    // 用 lua_pcall 调用一个函数
    // 2 是参数数量
    // 1 是返回值的数量
    // 0 是 lua 中的错误处理函数，这里不提供所以传 0
    // 检查 pcall 的返回值以捕捉调用错误
    if(lua_pcall(L, 1, 1, 0) != 0) {
        printf("LUA ERROR: %s\n", lua_tostring(L, -1));
    }
    // pcall 会吃掉栈中 3 个值（函数 参数1 参数2）
    // 然后 push 一个返回值进去
    //    dump(L);
    // luaadd 执行完毕后，返回值会被 push
    // lua_tonumber -1 的意思是把 栈顶 的元素转成数字，-1 是栈顶的下标
    // 如果 luaadd 返回的不是一个数字这里就会出错了
    // 所以一般来说要检查返回值
    // 所以有一个 lua_checknumber 函数包装了取值并检查的过程
    // 这里不管
    const char * n3 = lua_tostring(L, -1);
    //    int n3 = lua_tonumber(L, -1);
    lua_pop(L, 1);
    //    dump(L);
    //    printf("n3 %d\n", n3);
    return n3;
}



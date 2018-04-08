//
//  gwlua.c
//  server
//
//  Created by working on 2018/3/16.
//  Copyright © 2018年 working. All rights reserved.
//
/*
 gwlua 对 lua 封装
 C调用LUA文件中的函数方法
 
 lua_getglobal(L, <function name>) //获取lua中的函数
 lua_push*() //调用lua_push系列函数，把输入参数压栈。例如lua_pushnumber(L, x)
 lua_pcall(L, <arguments nums>, <return nums>, <错误处理函数地址>)
 
 调用函数
 首先，要调用的函数应该被压入堆栈；接着把需要传递给这个函数的参数按正序压栈；
 这是指第一个参数首先压栈。
 最后调用一下lua_call
 
 函数返回值
 函数返回值将按正序压栈（第一个返回值首先压栈），
 因此在调用结束后，最后一个返回值将被放在栈顶。
 
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


// 定义一个函数stackDump来打印当前栈的情况
static void
stackDump (lua_State *L) {
    int i;
    int top = lua_gettop(L);
    printf(".....begin dump lua stack.....\n");
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:  /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;
            default:  /* other values */
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("  ");  /* put a separator */
    }
    printf("\n.....end dump lua stack.....\n");  /* end the listing */
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
GuwLuaExeclTable(char *filename){
    if(luaL_dofile(L, filename)) {
        printf("啊LUA ERROR: %s\n", lua_tostring(L, -1));
        return "NULL";
    }
    
    lua_getglobal(L, "config");
    if (!lua_istable(L, -1)) {
        printf("error! me is not a table\n");
        return "NULL";
    }
//    lua_pushstring(L, arg);
//    if(lua_pcall(L, 0, 1, 0) != 0) {
//        printf("2 LUA ERROR: %s\n", lua_tostring(L, -1));
//    }
    
    // 把 t[k] 值压入堆栈， 这里的 t 是指有效索引 index 指向的值
    lua_getfield(L, -1, "host");
    printf("host %s\n", lua_tostring(L, -1));
    //把栈顶元素弹出去
    lua_pop(L, 1);
    
    // 把 t[k] 值压入堆栈， 这里的 t 是指有效索引 index 指向的值
    lua_getfield(L, -1, "port");

    stackDump(L);
    printf("port %lld\n", lua_tointeger(L, -1));
    
    lua_pop(L, 1);
    
    char *a = "aaa";
    return a;
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



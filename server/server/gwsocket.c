//
//  gwsocket.c
//  server
//

#include "gwsocket.h"
const static int BufferSize = 1024; // 缓冲区大小

static lua_State *L;

void
initLuaEnv(){
    
    // 创建 lua 运行环境
    L = luaL_newstate();
    // 加载 lua 标准库
    luaL_openlibs(L);
}

void
setNonBlock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        quit("fcntl()");
    }
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    
    if (r < 0) {
        quit("fcntl()");
    }
}


int
openSocket(unsigned short port) {
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
    
    if (s < 0) {
        quit("socket()");
    }
    
    // 消除端口占用错误
    const int option = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *)&option , sizeof(int));
    
    //
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    //
    bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(s, 5);
    //
    printf("listening at port %d\n", port);
    return s;
    
}



void *
printTask(void *arg) {
    int n = *(int *)arg;
    //    free(arg);
    printf("task %d \n", n);
    sleep(1);
    
    return NULL;
}


const char *
responseFromLua(lua_State *L, char *request) {
    int n1 = 1;
    int n2 = 2;
    //    dump(L);
    // 因为 L 已经 dofile 了 gua.lua
    // 所以 L 这个环境中有 luaadd 这个变量
    // getglobal 获取到的变量会 push 到栈
    // 所以现在其实 虚拟栈 中已经有一个数据了（函数 luaadd）
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


void *
response(void *socketFile){
    int s = *(int *)socketFile;
    
//    // 创建 lua 运行环境
    lua_State *L = luaL_newstate();
    // 加载 lua 标准库
    luaL_openlibs(L);
    
    // 载入 lua 文件并执行
    printf("response\n");
    if(luaL_dofile(L, "server.lua")) {
        printf("LUA ERROR: %s\n", lua_tostring(L, -1));
        return NULL;
    }
    
    char r[1024];
    read(s, r, sizeof(r));
    
    // add 函数中调用了一个 lua 中的函数
    const char *message = responseFromLua(L, r);
    
    lua_close(L);
    
    write(s , message , strlen(message));
//    printf("s: %u %s\n", (unsigned int)s, message);
    close(s);
    return NULL;
}


int
handleAccept(int socketFile){
    
    int s = socketFile;
    
    struct sockaddr_in raddr;
    socklen_t rsz = sizeof(raddr);
    struct sockaddr_in peer, local;
    socklen_t alen = sizeof(peer);
    
    int cfd = accept(s,(struct sockaddr *)&raddr, &rsz);
    int r = getpeername(cfd, (struct sockaddr*)&peer, &alen);
    
    return cfd;
}


void
handleRead(int socketFile){
    int s = socketFile;
    char buffer[BufferSize];
    
    read(s, buffer, sizeof(buffer));
//    printf("buffer %s\n", buffer);
    
    close(s);
}

void
closeLuaEnv(){
    lua_close(L);
}

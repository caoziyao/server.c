# server.c
server by c and lua
- 多进程，I/O 多路复用，线程池
- 支持 https
- 内存池对内存进行管理

Todo:
负载均衡

测试
```
siege -c 20 -r 1000 http://127.0.0.1:3000/
```
![siege](./screen/1.png)

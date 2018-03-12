单线程迭代阻塞处理
I/O multiplexing (select / poll, epoll)
进程、线程分派模型：one-request-per-child / one-request-per-thread
非阻塞 I/O，edge trigger

性能优化：
Memory caching
I/O 优化 如 文件系统选择、Linux AIO 等。单这块往下看文件系统、I/O 调度和其它 OS 内核细节，就可以挖非常深
TCP/IP 优化

CGI / FastCGI / WSGI / AJK 等后端协议支持
HTTPS 协议支持
模块化，扩展模块支持，这块更偏向于架构设计。Apache 就有非常出名的动态模块设计
集群支持


网络模式的方案:
- 阻塞 I/O（blocking IO）
- 非阻塞 I/O（nonblocking IO）
- I/O 多路复用（ IO multiplexing）
- 信号驱动 I/O（ signal driven IO）
- 异步 I/O（asynchronous IO）

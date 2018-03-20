方案选择:
- 阻塞 I/O（blocking IO）
- 非阻塞 I/O（nonblocking IO）
- I/O 多路复用（ IO multiplexing）
- 信号驱动 I/O（ signal driven IO）
- 异步 I/O（asynchronous IO）


todo：
- 反向代理
- Memory caching
- I/O 优化 如 文件系统选择、Linux AIO 等。单这块往下看文件系统、I/O 调度和其它 OS 内核细节
- TCP/IP 优化
- CGI / FastCGI / WSGI / AJK 等后端协议支持
- HTTPS 协议支持
- 模块化，扩展模块支持，这块更偏向于架构设计。nginx 设计
- 集群支持

------------
监控：
子进程收到终止信号后，完成自身善后工作，然后直接调用exit()终止。
父进程收到子进程终止的信号后，先调用waitpid()回收子进程的资源，避免僵尸进程，再拉起新的子进程。
父进程收到终止信号后，首先调用kill()通知子进程终止，然后调用waitpid()等待子进程终止并回收子进程的资源，最后父进程调用exit()终止。

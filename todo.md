方案选择:
- 阻塞 I/O（blocking IO）
- 非阻塞 I/O（nonblocking IO）
- I/O 多路复用（ IO multiplexing）
- 信号驱动 I/O（ signal driven IO）
- 异步 I/O（asynchronous IO）


性能优化：
- Memory caching
- I/O 优化 如 文件系统选择、Linux AIO 等。单这块往下看文件系统、I/O 调度和其它 OS 内核细节，
- TCP/IP 优化
- CGI / FastCGI / WSGI / AJK 等后端协议支持
- HTTPS 协议支持
- 模块化，扩展模块支持，这块更偏向于架构设计。Apache 就有非常出名的动态模块设计
- 集群支持

思路：
为了能让 cpu 接收到更多连接，完全不需要等待
io复用（kqueue）模拟异步
多线程多进程


服务器框架
	能够处理并发连接请求
	能够处理超时的连接（长时间无动作的连接要删除掉）
	消息的接收
	消息的发送
	发送失败或者接收失败的异常处理

HTTP协议处理模块
	HTTP协议解析
	HTTP协议构造

可以对日志信息的输出
	多线程
	消息队列
	日志的不同级别输出

业务逻辑



并发服务器框架
	TCP协议
	EPOLL 的使用（主要用来实现并发）
	多线程编程
	并发服务器处理模型

HTTP协议
	对HTTP协议要懂

日志系统
	锁
	信号量
	消息队列

业务逻辑
	消息到手自己想怎么处理就怎么处理了

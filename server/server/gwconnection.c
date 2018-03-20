//
//  gwconnection.c
//  server
// 封装 connection
/* create certs
 openssl genrsa -out privkey.pem 2048
 openssl req -new -x509 -key privkey.pem -out cacert.pem -days 1095
 */

#include "gwconnection.h"

#define FAIL    -1

// create server socket
GwConnection *
GwConnOpenSocket(unsigned short port) {
    
    GwConnection *conn = malloc(sizeof(GwConnection));
    
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
    
    // bind，成功返回0，出错返回-1
    if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
            perror("can't bind port\n");
            abort();
    }
    if (listen(s, 5) < 0) {
        perror("Can't configure listening port\n");
        abort();
    }
    //
    printf("listening at port %d\n", port);
    conn->server = s;
    conn->port = port;
    conn->ctx = NULL;
    conn->protocol = EnumProtocolHttp;
    
    return conn;
}


void
GwConnSetNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        quit("fcntl()1");
    }
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    
    if (r < 0) {
        quit("fcntl()2");
    }
}


void *
GwConnResponse(void *socketFile){
    int s = *(int *)socketFile;
    
    char r[MaxRecvBuff];
    read(s, r, sizeof(r));
    
    // add 函数中调用了一个 lua 中的函数
    const char *message = GuwLuaExecl("server_lua/server.lua", r);
    
    write(s , message , strlen(message));
    //    printf("s: %u %s\n", (unsigned int)s, message);
    close(s);
    return NULL;
}


// initialize SSL server  and create context
SSL_CTX *
GwConnInitServerCTX(void) {
//    SSL_METHOD *method;
    SSL_CTX *ctx;
    /* SSL 库初始化*/
    SSL_library_init();
    /* load & register all cryptos, etc. 载入所有SSL 算法*/
    OpenSSL_add_all_algorithms();
    /* load all error messages 载入所有SSL 错误消息*/
    SSL_load_error_strings();
    
    /* 以SSL V2 和V3 标准兼容方式产生一个SSL_CTX ，即SSL Content Text */
    ctx = SSL_CTX_new(SSLv23_server_method());
//    const SSL_METHOD *method = SSLv2_server_method();        /* create new server-method instance */
//    ctx = SSL_CTX_new(method);            /* create new context from method */
    if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    
    return ctx;
}


// 数字证书
void
LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile) {
    /* set the local certificate from CertFile */
    /* 载入用户的数字证书， 此证书用来发送给客户端。证书里包含有公钥*/
    if ( SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* set the private key from KeyFile (may be the same as CertFile) */
    /* 载入用户私钥*/
    if ( SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    /* 检查用户私钥是否正确*/
    if ( !SSL_CTX_check_private_key(ctx) ) {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}


// print out certificates
void
ShowCerts(SSL* ssl) {
    X509 *cert;
    char *line;
    
    cert = SSL_get_peer_certificate(ssl);    /* Get certificates (if available) */
    if ( cert != NULL ) {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else {
        printf("No certificates.\n");
    }
}


 /* Serve the connection -- threadable */
void
Servlet(SSL* ssl) {
    char buf[1024];
    int sd, bytes;
    
    /* do SSL-protocol accept 建立SSL 连接*/
    if ( SSL_accept(ssl) == FAIL ) {
        perror("Servlet SSL_accept\n");
        ERR_print_errors_fp(stderr);
        
    }  else {
        ShowCerts(ssl);                                /* get any certificates */
        /* get request 接收服务器来的消息 */
        bytes = SSL_read(ssl, buf, sizeof(buf));
        if ( bytes > 0 ) {
            buf[bytes] = 0;

            // add 函数中调用了一个 lua 中的函数
            const char *message = GuwLuaExecl("server_lua/server.lua", buf);
            printf("msg %s\n", message);
            /* 发消息给服务器 */
            SSL_write(ssl, message, strlen(message));    /* send reply */
            
        } else {
            perror("Servlet bytes\n");
            ERR_print_errors_fp(stderr);
        }
    }
    sd = SSL_get_fd(ssl);                            /* get socket connection */
    SSL_free(ssl);                                    /* release SSL state */
    close(sd);                                        /* close connection */
}


// create server socket
GwConnection *
GwConnSSLOpenSocket(unsigned short port) {
    
    SSL_CTX *ctx;
    GwConnection *conn = malloc(sizeof(GwConnection));
    
    /* initialize SSL */
    ctx = GwConnInitServerCTX();
    LoadCertificates(ctx, "cacert.pem", "privkey.pem");
    
    GwConnection *s = GwConnOpenSocket(port);
    
    conn->server = s->server;
    conn->port = port;
    conn->ctx = ctx;
    conn->protocol = EnumProtocolHttps;

    return conn;
}


 /* release context */
void
GwConnSSLFree(GwConnection *conn) {
    SSL_CTX *ctx = conn->ctx;
    if (ctx != NULL) {
        SSL_CTX_free(ctx);
    }
}


int
GwConnSSLTest() {
    SSL_CTX *ctx;
    int server;
    unsigned int port = 3001;
    
    ctx = GwConnInitServerCTX();
    LoadCertificates(ctx, "cacert.pem", "privkey.pem");  
    GwConnection *conn = GwConnOpenSocket(port);
    server = conn->server;
    while (1) {
        struct sockaddr_in addr;
        int len = sizeof(addr);
        SSL *ssl;

        int client = accept(server, NULL, NULL);
//        int client = accept(server, &addr, &len);        /* accept connection as usual */
        printf("Connection: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        /* 基于ctx 产生一个新的SSL */
        ssl = SSL_new(ctx);
        /* 将新连接的socket 加入到SSL */
        SSL_set_fd(ssl, client);
        Servlet(ssl);
    }
    close(server);
    SSL_CTX_free(ctx);
    return 0;
}

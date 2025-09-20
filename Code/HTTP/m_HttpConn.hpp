#ifndef HTTPCONN_H
#define HTTPCONN_H
#include "http_request.hpp"
#include "http_response.hpp"
#include <arpa/inet.h> 
#include <unistd.h>
class m_HttpConn{
public:
    m_HttpConn();
    m_HttpConn(int sockfd_, sockaddr_in addr_);
    ~m_HttpConn();
    bool read();
    bool write();
    bool process();
    int getid();
    //资源目录
    static const char* srcDir; 
private:
    int sockfd;
    sockaddr_in addr;
    //该连接的请求
    http_request request;
    //该连接的响应
    http_response response;
    //为该连接申请的读缓冲区
    Buffer read_buff;
    //为该链接申请的写缓冲区
    Buffer write_buff;
};
#endif
#include "http_request.hpp"
#include "http_response.hpp"
#include <arpa/inet.h> 
class m_HttpConn{
public:
    m_HttpConn();
    ~m_HttpConn();
    void read();
    void write();
    void process();


private:
    int fd;
    sockaddr_in addr;
    
    //该连接的请求
    http_request request;
    //该连接的响应
    
    //为该连接申请的读缓冲区
    Buffer read_buff;
    //为该链接申请的写缓冲区
    Buffer write_buff;


};
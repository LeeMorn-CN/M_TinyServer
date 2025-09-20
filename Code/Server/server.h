#ifndef SERVER_H
#define SERVER_H
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include "../Pool/m_threadpool.hpp"
#include "../Server/m_epoll.h"
#include "../HTTP/m_HttpConn.hpp"
 
class WebServer{
public:
    WebServer();
    ~WebServer();
    void Init_Listensocket();
    void AddUser();
    void DelUser(int sock_id);
    void OnRead(m_HttpConn& client);
    void OnWrite(m_HttpConn& client);
    int set_noblock(int fd);
private:
    //监听ID
    int listenfd;
    //epoll返回发生变化的事件数量
    int events_num;
    // 源地址目录
    char* srcDic_;
    //服务器ip+port
    sockaddr_in m_host;
    std::unique_ptr<ThreadPool> m_pool;
    std::unique_ptr<M_epoll> m_epoll;
     //连接用户
    std::unordered_map<int,m_HttpConn> Users;
};
#endif
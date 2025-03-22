#include <arpa/inet.h>
#include "../Pool/m_threadpool.hpp"
#include "../Server/m_epoll.h"
 
class WebServer{
public:
    WebServer();
    ~WebServer();
    void Init_Listensocket();
    int set_noblock(int fd);
private:
    //监听ID
    int listenfd;
    //epoll返回发生变化的事件数量
    int events_num;
    //服务器ip+port
    sockaddr_in m_host;
    std::unique_ptr<ThreadPool> m_pool;
    std::unique_ptr<M_epoll> m_epoll;
};
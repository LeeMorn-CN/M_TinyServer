#include "server.h"
#include <fcntl.h>
using namespace std;
WebServer::WebServer():m_host(sockaddr_in{}){
    Init_Listensocket();
    m_epoll = make_unique<M_epoll>();
    m_pool = make_unique<ThreadPool>();
    m_epoll->add(listenfd,EPOLLIN);
    while(1){
        events_num = m_epoll->wait();
        int sock_id;
        for(int i=0; i<events_num; i++){
            sock_id = m_epoll->getfd(i);
            //新连接产生
            if(sock_id==listenfd){
                sockaddr_in client_info = sockaddr_in{};
                socklen_t addrlen = sizeof(client_info);
                int clientfd = accept(listenfd,(sockaddr*)&client_info,&addrlen);
                set_noblock(clientfd);
                m_epoll->add(clientfd,EPOLLIN|EPOLLOUT|EPOLLERR);
            }
            //已连接客户端产生读写事件
            else{
                uint32_t cur_ev = m_epoll->getevent(sock_id);
                if(cur_ev&EPOLLIN){
                    //向线程池加入读任务
                } 
                if(cur_ev&EPOLLOUT){
                    //向线程池加入写任务
                }
                if(cur_ev&EPOLLERR){
                    //断开连接
                    m_epoll->del(sock_id);
                    close(sock_id);
                }
            }
        }
    }
}
WebServer::~WebServer(){
    close(listenfd);
}
void WebServer::Init_Listensocket(){
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    m_host.sin_family = AF_INET;
    m_host.sin_addr.s_addr = INADDR_ANY;
    m_host.sin_port = htons(80);
    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    bind(listenfd,(sockaddr*)&m_host,sizeof(m_host));
    listen(listenfd,128);
}
int WebServer::set_noblock(int fd){
    int flag = fcntl(fd,F_GETFL);
    return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}
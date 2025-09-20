#include "server.h"
using namespace std;
WebServer::WebServer():m_host(sockaddr_in{}){
    Init_Listensocket();
    m_epoll = make_unique<M_epoll>();
    m_pool = make_unique<ThreadPool>();
    m_epoll->add(listenfd,EPOLLIN);
    srcDic_ = getcwd(nullptr, 256);
    m_HttpConn::srcDir = srcDic_;
    while(1){
        events_num = m_epoll->wait();
        int sock_id;
        for(int i=0; i<events_num; i++){
            sock_id = m_epoll->getfd(i);
            //新连接产生
            if(sock_id==listenfd){
                AddUser();
            }
            //已连接客户端产生读写事件
            else{
                uint32_t cur_ev = m_epoll->getevent(i);
                if(cur_ev&EPOLLIN){
                    //向线程池加入读任务
                    m_pool->append_task([&]{ OnRead(Users[sock_id]); });
                    cout<<"加入读任务"<<endl;
                } 
                else if(cur_ev&EPOLLOUT){
                    //向线程池加入写任务
                    m_pool->append_task([&]{ OnWrite(Users[sock_id]); });
                    cout<<"加入写任务"<<endl;
                }
                else if(cur_ev&EPOLLERR){
                    //断开连接
                    DelUser(sock_id);
                    cout<<"某个连接断开"<<endl;
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
    inet_pton(AF_INET, "172.27.19.111", &m_host.sin_addr.s_addr);
    m_host.sin_port = htons(8080);
    int opt = 1;
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    bind(listenfd,(sockaddr*)&m_host,sizeof(m_host));
    listen(listenfd,128);
}
int WebServer::set_noblock(int fd){
    int flag = fcntl(fd,F_GETFL);
    return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}
void WebServer::AddUser(){
    sockaddr_in client_info = sockaddr_in{};
    socklen_t addrlen = sizeof(client_info);
    int clientfd = accept(listenfd, (sockaddr*)&client_info, &addrlen);
    Users.emplace(clientfd, m_HttpConn(clientfd, client_info));
    set_noblock(clientfd);
    m_epoll->add(clientfd, EPOLLIN|EPOLLERR|EPOLLET);
    cout<<"新连接产生，加入该用户:"<<clientfd<<endl;
}
void WebServer::DelUser(int sock_id){
    m_epoll->del(sock_id);
    Users.erase(sock_id);
    close(sock_id);
}
void WebServer::OnRead(m_HttpConn& client){
    client.read();
    client.process();
    m_epoll->mod(client.getid(), EPOLLOUT|EPOLLERR|EPOLLET);
}
void WebServer::OnWrite(m_HttpConn& client){
    if(client.write()){
        m_epoll->mod(client.getid(), EPOLLIN|EPOLLERR|EPOLLET);
    }
    else{
        m_epoll->mod(client.getid(), EPOLLOUT|EPOLLERR|EPOLLET);
    }
}

#include "m_epoll.h"
using namespace std;
#define MAX_FD 65536
//构造
M_epoll::M_epoll():epoll_fd(epoll_create(1)),events(1024){}
//析构
M_epoll::~M_epoll(){
    close(epoll_fd);
}
//添加
bool M_epoll::add(int fd, uint32_t event){
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = event;
    return epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev)==0;   
}
//删除
bool M_epoll::del(int fd){
    epoll_event ev = {0};
    return epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,&ev)==0;
}
//修改
bool M_epoll::mod(int fd, uint32_t event){
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = event;
    return epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&ev)==0;
}
//等待事件产生
int M_epoll::wait(int timeoutMs = -1){
    int ev_num = epoll_wait(epoll_fd,&events[0],static_cast<int>(events.size()),timeoutMs);
    //动态扩容
    if(ev_num > events.size() * 0.8 && ev_num * 2 <= MAX_FD)events.resize(2 * ev_num);
    return ev_num;
}
//获取变化的文件描述符
int  M_epoll::getfd(int i){
    return events[i].data.fd;
}
//获取事件类型
uint32_t  M_epoll::getevent(int i){
    return events[i].events;
}
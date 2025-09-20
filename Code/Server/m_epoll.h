#ifndef M_EPOLL_H
#define M_EPOLL_H
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
class M_epoll{
public:
    M_epoll();
    ~M_epoll();
    bool add(int fd, uint32_t event);
    bool del(int fd);
    bool mod(int fd, uint32_t event);
    int wait(int timeoutMs = -1);
    int getfd(int i);
    uint32_t getevent(int i);
private:
    int epoll_fd;
    std::vector<epoll_event> events;
    
};
#endif
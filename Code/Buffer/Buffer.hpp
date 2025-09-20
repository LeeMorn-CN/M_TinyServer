#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string>
class Buffer{
public:
    Buffer(int initBuffsize = 1024);
    bool readfd(int fd);
    bool writefd(int fd);
    size_t usable_size();
    size_t readable_size();
    void BuffResize(size_t size);
    void retrieve_all();
    void retrieve(size_t size);
    const char* read_BuffPos();
    char* write_BuffPos();
    void append(const char* str, size_t len);
    void append(const std::string& str, size_t len);
    void EnsureSpace(size_t len);
private:
    std::vector<char> buff;
    size_t head_pos;
    size_t end_pos;

};
#endif
#include <vector>
#include <arpa/inet.h>
class Buffer{
public:
    Buffer(int initBuffsize = 1024);
    void readfd(int fd);
    void writefd(int fd);
    size_t usable_size();
    void BuffResize(size_t size);
    void retrieve();

private:
    std::vector<char> buff;
    size_t head_pos;
    size_t end_pos;

};
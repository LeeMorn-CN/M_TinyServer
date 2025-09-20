#include "Buffer.hpp"
Buffer::Buffer(int initBuffsize ):buff(initBuffsize), head_pos(0), end_pos(0){}

bool Buffer::readfd(int fd){
    int len = 0; 
    ssize_t ret = ioctl(fd, FIONREAD, &len);
    //后期日志考虑出错
    if(len == 0){
        return false;
    }
    else {
        EnsureSpace(len);
    }
    size_t able_size = usable_size();
    len = recv(fd, buff.data() + end_pos, able_size, 0);
    end_pos += static_cast<size_t>(len);
    return true;
}
bool Buffer::writefd(int fd){
    ssize_t ret = send(fd, buff.data() + head_pos, end_pos - head_pos, 0);
    if(ret<0){
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            return false;
        }
    }
    else{
        head_pos +=static_cast<size_t>(ret);
    }
    return true;
}
size_t Buffer::usable_size(){
    return buff.size()-end_pos;
}
void Buffer::BuffResize(size_t size){
    buff.resize(size);
}
//清空缓冲区
void Buffer::retrieve_all(){
    //数据头部位置
    head_pos = 0;
    //数据尾部的下一个位置
    end_pos =0;
}
const char* Buffer :: read_BuffPos(){
    return buff.data() + head_pos;
}
char* Buffer :: write_BuffPos(){
    return buff.data() + end_pos;
}
size_t Buffer :: readable_size(){
    return end_pos - head_pos;
}
void Buffer :: retrieve(size_t size){
    head_pos += size;
}
void Buffer::append(const std::string& str, size_t len){
    EnsureSpace(len);
    std::copy(str.data(), str.data() + len, write_BuffPos());
    end_pos += len;
}
void Buffer::append(const char* str, size_t len){
    EnsureSpace(len);
    std::copy(str, str+len, write_BuffPos());
    end_pos += len;
}
void Buffer::EnsureSpace(size_t len){
    size_t able_size = usable_size();
    if(able_size < len){
        BuffResize(end_pos + len);
    }
}
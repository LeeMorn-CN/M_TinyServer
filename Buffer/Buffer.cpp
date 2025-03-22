#include "Buffer.hpp"
Buffer::Buffer(int initBuffsize ):buff(initBuffsize), head_pos(0), end_pos(0){}

void Buffer::readfd(int fd){
    size_t able_size = usable_size();
    ssize_t ret = recv(fd, buff.data() + end_pos, able_size, MSG_PEEK);
    //后期日志考虑出错
    if(ret<0){
        //出错
    }
    else if(able_size < static_cast<size_t>(ret)){
        BuffResize(end_pos + static_cast<size_t>(ret));
        able_size = ret;
    }
    ret = recv(fd, buff.data() + end_pos, able_size, 0);
    end_pos += static_cast<size_t>(ret);
}
void Buffer::writefd(int fd){
    ssize_t ret = send(fd, buff.data(), usable_size(), 0);
    if(ret<0){
        //错误处理
    }
    else{
        head_pos +=static_cast<size_t>(ret);
    }
}
size_t Buffer::usable_size(){
    return buff.size()-end_pos;
}
void Buffer::BuffResize(size_t size){
    buff.resize(size);
}
//清空缓冲区
void Buffer::retrieve(){
    //数据头部位置
    head_pos = 0;
    //数据尾部的下一个位置
    end_pos =0;
}

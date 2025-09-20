#include "m_HttpConn.hpp"
const char* m_HttpConn::srcDir;
m_HttpConn::m_HttpConn(){}
m_HttpConn::m_HttpConn(int sockfd_, sockaddr_in addr_): sockfd(sockfd_), addr(addr_){}
m_HttpConn::~m_HttpConn(){
}
bool m_HttpConn :: read(){
    while(1){
        if(!read_buff.readfd(sockfd)){
            break;
        }
    }
    return true;
}
bool m_HttpConn :: write(){
    while(1){
        if(!write_buff.writefd(sockfd)){
            if(write_buff.read_BuffPos() == write_buff.write_BuffPos()){
                break;
            }
            return false;
        }
    }
    return true;
}
bool m_HttpConn::process(){
    if(!request.ParseRequest(read_buff)){
        return false;
    }
    response.Init(-1, srcDir, request.path_(), request.isKeepLive());
    response.Response(write_buff);
    return true;
}
int m_HttpConn::getid(){
    return sockfd;
}
#include "http_response.hpp"
using namespace std;
const unordered_map<int, string> http_response::CODE_STATUS = {
    // 成功系列 2xx
    { 200, "OK" },                  // 请求成功    
    { 204, "NO_CONTENT" },          // 请求成功，但无BODY
    { 206, "PARTIAL_CONTENT" },     //请求成功，返回部分BODY
    // 重定向系列 3xx
    { 301, "MOVED_PERMANENTLY" },    // 资源已永久移动到新地址
    { 302, "FOUND" },                // 资源暂时移动
    { 304, "NOT_MODIFIED" },         // 资源未修改
    // 客户端错误系列 4xx    
    { 400, "Bad Request" },         // 请求无效
    { 403, "Forbidden" },           // 请求被禁止
    { 404, "Not Found" },           // 请求的资源不存在
    // 服务器错误系列 5xx
    { 500, "INTERNAL_SERVER_ERROR" },  // 服务器内部错误
    { 501, "NOT_IMPLEMENTED" },        // 服务器不支持当前请求的方法
    { 502, "BAD_GATEWAY" },            // 错误的网关
    { 503, "SERVICE_UNAVAILABLE" }    // 服务不可用
    // 自定义
    //NO_REQUEST = 1000,          // 请求尚未到达（还没有请求）
    //GET_REQUEST = 1001,         // 处理 GET 请求
    //FILE_REQUEST = 1002,        // 请求的文件资源
    //CLOSED_CONNECTION = 1003,   //关闭连接 
};
http_response::~http_response(){
    munmap(FilePrt, Filestat.st_size);
}
void http_response::Init(int code_, const string& srcDic_, string& path_, bool isKeepAlive_){
    code = code_;
    srcDic = srcDic_;
    path = path_;
    isKeepAlive = isKeepAlive_;
}
void http_response::Response(Buffer& buff){
    if(path!= "/"&&(stat((srcDic + path).data(), &Filestat) < 0 ||S_ISDIR(Filestat.st_mode))){
        code = 404;
    }
    else if(path!="/" && !(Filestat.st_mode & S_IROTH)) {
        code = 403;
    }
    else{ 
        code = 200; 
    }
    AddStateLine(buff);
    AddHeader(buff);
    AddBody(buff);
}
void http_response::AddStateLine(Buffer& buff){
    string status;
    if(CODE_STATUS.count(code) == 1) {
        status = CODE_STATUS.at(code);
    }
    else {
        code = 400;
        status = CODE_STATUS.at(code);
    }
    string Line = "HTTP/1.1 " + to_string(code) + " " + status + "\r\n";
    buff.append(Line, Line.size());
}
void http_response::AddHeader(Buffer& buff){
    string header = "Connection: ";
    if(isKeepAlive) {
        header += "keep-alive\r\n";
    } else{
        header += "close\r\n";
    }
    buff.append(header, header.size());
    header.clear();
    header = "Content-type: image/jpeg\r\n";
    buff.append(header, header.size());
}
void http_response::AddBody(Buffer& buff){
    int filefd = open((srcDic + path).data(), O_RDONLY);
    FilePrt =(char*)mmap(0, Filestat.st_size, PROT_READ, MAP_PRIVATE, filefd, 0);
    string content_len = "Content-length: " + to_string(Filestat.st_size) + "\r\n\r\n";
    buff.append(content_len, content_len.size());
    buff.append(FilePrt, static_cast<size_t>(Filestat.st_size));
    close(filefd);
}
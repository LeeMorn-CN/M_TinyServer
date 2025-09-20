#ifndef RESPONSE_H
#define RESPONSE_H
#include <string>
#include <unordered_map>
#include <algorithm>
#include <sys/stat.h> 
#include <sys/mman.h>
#include <regex>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "../Buffer/Buffer.hpp"
class http_response{
public:
    http_response() = default;
    ~http_response();
    void Init(int code_, const std::string& srcDic_, std::string& path_, bool isKeepAlive_);
    void Response(Buffer& buff);
    void AddStateLine(Buffer& buff);
    void AddHeader(Buffer& buff);
    void AddBody(Buffer& buff);
private:
    //状态吗
    int code;
    std::string srcDic;
    std::string path;
    bool isKeepAlive;
    //内存映射指针
    char* FilePrt;
    //文件状态
    struct stat Filestat;
    static const std::unordered_map<int, std::string> CODE_STATUS;

};
#endif
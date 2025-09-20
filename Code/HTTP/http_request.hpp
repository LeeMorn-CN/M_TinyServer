#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <unordered_map>
#include <algorithm>
#include <regex>
#include "../Buffer/Buffer.hpp"

class http_request{
public:
    enum class PARSE_STATE{
        REQUEST_LINE,   //请求首行
        HEADERS,        //头部
        BODY,           //体
        FINISH,         //完成

    };
    http_request();
    ~http_request();
    bool ParseRequest_Line(const std::string& line);
    bool ParseRequest_Header(const std::string& line);
    bool ParseRequest_Body(const std::string& line);
    bool ParseRequest(Buffer& buff);
    bool isKeepLive();
    std::string& path_();



private:
    PARSE_STATE state;
    std::string method, path, version;
    std::unordered_map<std::string, std::string> header;
};
#endif
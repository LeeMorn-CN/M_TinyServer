#include <string>
#include <unordered_map>
#include "../Buffer/Buffer.hpp"
class http_request{
public:
    enum class PARSE_STATE{
        REQUEST_LINE,   //请求首行
        HEADERS,        //头部
        BODY,           //体
        FINISH,         //完成

    };
    enum class HTTP_CODE {
        // 成功系列 2xx
        OK = 200,                     // 请求成功             
        NO_CONTENT = 204,             // 请求成功，但无BODY
        PARTIAL_CONTENT = 206,        //请求成功，返回部分BODY
    
        // 重定向系列 3xx
        MOVED_PERMANENTLY = 301,      // 资源已永久移动到新地址
        FOUND = 302,                  // 资源暂时移动
        NOT_MODIFIED = 304,           // 资源未修改
    
        // 客户端错误系列 4xx
        BAD_REQUEST = 400,            // 请求无效
        FORBIDDEN = 403,              // 请求被禁止
        NOT_FOUND = 404,              // 请求的资源不存在

        // 服务器错误系列 5xx
        INTERNAL_SERVER_ERROR = 500,  // 服务器内部错误
        NOT_IMPLEMENTED = 501,        // 服务器不支持当前请求的方法
        BAD_GATEWAY = 502,            // 错误的网关
        SERVICE_UNAVAILABLE = 503,    // 服务不可用

        // 自定义
        NO_REQUEST = 1000,          // 请求尚未到达（还没有请求）
        GET_REQUEST = 1001,         // 处理 GET 请求
        FILE_REQUEST = 1002,        // 请求的文件资源
        CLOSED_CONNECTION = 1003,   //关闭连接 
    };  
    http_request();
    ~http_request();
    void ParseRequest_Line();
    void ParseRequest_Header();
    void ParseRequest_Body();
    void ParseRequest(Buffer& buff);



private:
    PARSE_STATE status;
    std::string method, path, version;
    std::unordered_map<std::string, std::string> header;
};
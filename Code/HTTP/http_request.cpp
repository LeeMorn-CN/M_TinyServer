#include "http_request.hpp"
using namespace std;
http_request::http_request(){
    state = PARSE_STATE::REQUEST_LINE;
}
http_request::~http_request(){

}
bool http_request::ParseRequest(Buffer& buff){
    const char CRLF[] = "\r\n";
    if(buff.readable_size()<=0){
        return false;
    }
    else {
        while(buff.readable_size()>0&&state!= PARSE_STATE::FINISH){
            const char* LineEnd =  search(buff.read_BuffPos(), (const char*)buff.write_BuffPos(), CRLF,CRLF+2);
            const string Line(buff.read_BuffPos(),LineEnd);
            switch (state)
            {
                case PARSE_STATE::REQUEST_LINE :
                    ParseRequest_Line(Line);
                    break;
                case PARSE_STATE::HEADERS :
                    ParseRequest_Header(Line);
                    break;
    
                case PARSE_STATE::BODY :
                    ParseRequest_Body(Line);
                    break;
                default :
                    break;
            }
            buff.retrieve(static_cast<size_t>(Line.size()+2));
        }
    }
    return true;
}
bool http_request::ParseRequest_Line(const string& line){
    regex pattern("^(\\S+) (\\S+) HTTP/(\\S+)$");
    smatch subMatch;
    if(regex_match(line, subMatch, pattern)){
        method = subMatch[1];
        path = subMatch[2];
        version = subMatch[3];
        state = PARSE_STATE::HEADERS;
        return true;
    }
    return false;
}
bool http_request::ParseRequest_Header(const string& line){
    regex pattern("^([^:]*): ?(.*)$");
    smatch subMatch;
    if(regex_match(line, subMatch, pattern)){
        header[subMatch[1]] = subMatch[2];
    }
    else{
        state = PARSE_STATE::BODY;
    }
    return true;

}
bool http_request::ParseRequest_Body(const string& line){
    //后期实现post请求
    state = PARSE_STATE::FINISH;
    return true;
}
bool http_request::isKeepLive(){
    if(header.count("Connection") == 1){
        return header["Connection"] == "keep-alive";
    }
    return false;
}
string& http_request::path_() {
    return path;
}
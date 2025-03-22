#include "http_request.hpp"
http_request::http_request(){
    status = PARSE_STATE::REQUEST_LINE;
}
http_request::~http_request(){

}
void http_request::ParseRequest(Buffer& buff){

    switch (status)
    {
    case PARSE_STATE::REQUEST_LINE :
        
        break;
    
    case PARSE_STATE::HEADERS :
        break;
    
    case PARSE_STATE::BODY :
        break;
    case PARSE_STATE::FINISH :
        break;
    }

}
void http_request::ParseRequest_Line(){

}
void http_request::ParseRequest_Header(){

}
void http_request::ParseRequest_Body(){

}

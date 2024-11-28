// ответ
# pragma once

#include <string>
#include <vector>
#include <functional>

#include "uart_uno.hpp"
#include "smart_home.hpp"

const char OK[] = "HTTP/1.1 200 Ok\n";
const char FORBIDDEN[] = "HTTP/1.1 403 Forbidden\n";
const char NOT_FOUND[] = "HTTP/1.1 404 Not_Found\n";
const char SERVER_ERROR[] = "HTTP/1.1 500 Internal_Server_Error\n";

const char Content_Length[] = "Content-Length";

class Response {
    public:
        enum class StatusCode {OK, FORBIDDEN, NOT_FOUND, SERVER_ERROR};
        
        Response() {}
        ~Response() {}
        // получение итогового ответа
        std::string GET_result();
        void SET_status_code_200();
        void SET_status_code_403();
        void SET_status_code_404();
        void SET_status_code_500();
        void SET_headlines(std::string key, std::string value);
        void Upload_text_file(std::string file_name);
        void Upload_text_file(std::string file_name, SmartHome & smart_home, std::function<std::string(std::string&, SmartHome&)> func);
        void Upload_binary_file(std::string file_name);
    private:
        std::string response_body = ""; 
        std::string response_headers;
        StatusCode status_code;
        // заголовки ответа
        std::vector<std::string> headlines;
};
// запрос
# pragma once

#include <unordered_map>
#include <string>

class Request {
    public:
        Request(char * buffer, int bytes_read);
        ~Request() {}
        enum class Method {GET, POST, HEAD, PUT, DELETE, OPTIONS, PATCH, ERROR};
        int GET_begin_request_body() const {return begin_request_body;}
        Method GET_method() const {return method;}
        const std::string & GET_url() const {return url;}
        const std::string & GET_version() const {return version;}
        std::string GET_headlines(const std::string& key) const;
        void Show_RequestHeders() const;
    private:
        // начало тела
        int begin_request_body = 0;
        // метод запроса
        Method method;
        // url
        std::string url;
        // версия http
        std::string version;
        // заголовки
        std::unordered_map<std::string, std::string> headlines;
};
// ответ
# pragma once

#include <string>

#include "request.hpp"

class Response {
    public:
        const std::string PATH = "../src/"; // каталог с файлами фронтенда

        const std::string OK = "HTTP/1.1 200 Ok\n";
        const std::string FORBIDDEN = "HTTP/1.1 403 Forbidden\n";
        const std::string NOT_FOUND = "HTTP/1.1 404 Not_Found\n";
        const std::string SERVER_ERROR = "HTTP/1.1 500 Internal_Server_Error\n";

        const std::string Content_Type = "Content-Type: ";
            const std::string text_html = "text/html\n";
            const std::string text_css = "text/css\n";
            const std::string text_js = "text/js\n";
            const std::string image_icon = "image/x-icon\n";
            const std::string image_jpeg = "image/jpeg\n";

        const std::string Content_Length = "Content-Length: ";

        enum class TypeURL {
            TEXT, FILE, ERROR
        };

        Response(Request & request_class);
        //Response(Request & request_class, RequestBody & req_body_class);
        ~Response() {}
        // получение итогового ответа
        std::string GET_result_headers() {return result_headers;}
        std::string GET_response_body() {return response_body;}
    private:
        std::string response_body; // тело ответа
        std::string response_headers; // заголовки ответа
        std::string cod; // статус код
        std::string result_headers; // итоговый ответ
        // проверка валидности url
        bool isValidUrl(const std::string& url);
        // парсер url и настройка параметров
        void pars_url(std::string& url, Response::TypeURL & type_url);
};
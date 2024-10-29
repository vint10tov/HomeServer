#include <iostream>
#include <algorithm>

#include "response.hpp"
#include "text_response.hpp"
#include "file_response.hpp"

Response::Response(Request & request_class) {
    auto url = request_class.GET_url();
    if (isValidUrl(url)) {
        Response::TypeURL tu;
        pars_url(url, tu);
        if (tu == Response::TypeURL::TEXT) {
            
            TextResponse text_resp(url);
            if (text_resp.GET_flag_error()) {
                response_body = text_resp.GET_file_buffer();
                struct stat st = text_resp.GET_file_stat();
                response_headers += (Content_Length + std::to_string(st.st_size) + "\n");
                cod = OK;
            } else {
                cod = SERVER_ERROR;
            }
        } else if (tu == Response::TypeURL::FILE) {
            FileResponse file_resp(url);
            if (file_resp.GET_flag_error()) {
                response_body = file_resp.GET_file_buffer();
                struct stat st = file_resp.GET_file_stat();
                response_headers += (Content_Length + std::to_string(st.st_size) + "\n");
                cod = OK;
            } else {
                cod = SERVER_ERROR;
            }
        }
    } else {
        cod = NOT_FOUND;
    }
    if (cod == OK) {
        result_headers = cod + response_headers + "\n";
    } else {
        result_headers = cod;
        response_body = "";
    }
}
// проверка валидности url
bool Response::isValidUrl(const std::string& url){
    return std::all_of(url.begin(), url.end(), [](char c) {
        return std::isalpha(c) || std::isdigit(c) || c == '/' || c == '.';
        });
}
// парсер url и настройка параметров
void Response::pars_url(std::string& url, Response::TypeURL & type_url) {
    if (url == "/") {
        url = PATH + "html/index.html";
        type_url = TypeURL::TEXT;
        response_headers += (Content_Type + text_html);
    } else if (url == "/favicon.ico") {
        url = PATH + "image/favicon.ico";
        type_url = TypeURL::FILE;
        response_headers += (Content_Type + image_icon);
    } else if (url.compare(0, 7, "/image/") == 0) {
        size_t pos = url.find_last_of('.');
        if (pos != std::string::npos) {
            std::string temp1 = url.substr(pos + 1);  // Возвращаем подстроку после точки
            std::string temp2 = PATH + url;
            url = temp2;
            type_url = TypeURL::FILE;
            response_headers += (Content_Type + "image/" + temp1 + "\n");
        } else {
            cod = SERVER_ERROR;
        }
    } else if (url.compare(0, 5, "/css/") == 0) {
        std::string temp = PATH + url;
        url = temp;
        type_url = TypeURL::TEXT;
        response_headers += (Content_Type + text_css);
    } else if (url.compare(0, 4, "/js/") == 0) {
        std::string temp = PATH + url;
        url = temp;
        type_url = TypeURL::TEXT;
        response_headers += (Content_Type + text_js);
    } else {
        std::string temp = PATH + url + ".html";
        url = temp;
        type_url = TypeURL::TEXT;
        response_headers += (Content_Type + text_html);
    }
}
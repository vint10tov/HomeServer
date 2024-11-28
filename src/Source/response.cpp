#include "response.hpp"
#include "file_to_string.hpp"
#include "text_to_string.hpp"
#include "uart_uno.hpp"

std::string Response::GET_result() {
    if (status_code == StatusCode::OK)
        response_headers = OK;
    else if (status_code == StatusCode::FORBIDDEN)
        response_headers = FORBIDDEN;
    else if (status_code == StatusCode::NOT_FOUND)
        response_headers = NOT_FOUND;
    else
        response_headers = SERVER_ERROR;
    
    for (const auto& element : headlines) {
        response_headers += element;
    }
    return response_headers + "\n" + response_body;
}

void Response::SET_status_code_200() {
    status_code = StatusCode::OK;
}

void Response::SET_status_code_403() {
    status_code = StatusCode::FORBIDDEN;
}

void Response::SET_status_code_404() {
    status_code = StatusCode::NOT_FOUND;
}

void Response::SET_status_code_500() {
    status_code = StatusCode::SERVER_ERROR;
}

void Response::SET_headlines(std::string key, std::string value) {
    headlines.push_back(key + ": " +value + "\n");
}

void Response::Upload_text_file(std::string file_name) {
    TextToString tts(file_name);
    if (tts.GET_flag_error()) {
        response_body = tts.GET_file_buffer();
        SET_headlines(Content_Length, std::to_string(response_body.size()));
    } else {
        status_code = StatusCode::SERVER_ERROR;
    }
}

void Response::Upload_text_file(std::string file_name, SmartHome & smart_home,std::function<std::string(std::string&, SmartHome&)> func) {
    TextToString tts(file_name);
    if (tts.GET_flag_error()) {
        std::string temp = tts.GET_file_buffer();
        response_body = func(temp, smart_home);
        SET_headlines(Content_Length, std::to_string(response_body.size()));
    } else {
        status_code = StatusCode::SERVER_ERROR;
    }
}

void Response::Upload_binary_file(std::string file_name) {
    FileToString fts(file_name);
    if (fts.GET_flag_error()) {
        response_body = fts.GET_file_buffer();
        SET_headlines(Content_Length, std::to_string(response_body.size()));
    } else {
        status_code = StatusCode::SERVER_ERROR;
    }
}
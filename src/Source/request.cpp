#include <cstring>
#include <iostream>

#include "request.hpp"

Request::Request(char * buffer, int bytes_read) {

    // определение метода запроса
    if (strncmp(buffer, "GET", 3) == 0) method = Method::GET;
    else if (strncmp(buffer, "POST", 4) == 0) method = Method::POST;
    else if (strncmp(buffer, "HEAD", 4) == 0) method = Method::HEAD;
    else if (strncmp(buffer, "PUT", 3) == 0) method = Method::PUT;
    else if (strncmp(buffer, "DELETE", 6) == 0) method = Method::DELETE;
    else if (strncmp(buffer, "OPTIONS", 7) == 0) method = Method::OPTIONS;
    else if (strncmp(buffer, "PATCH", 5) == 0) method = Method::PATCH;
    else method = Method::ERROR;

     // номер позиции перебора массива буфера
    int i = 0;
    // первый пробел
    int space_1 = 0;
    // второй пробел
    int space_2 = 0;
    
    // ищим пробелы
    while (buffer[i] != ' ' && i < bytes_read) ++i;
    if (i < bytes_read) space_1 = i++;
    while (buffer[i] != ' ' && i < bytes_read) ++i;
    if (i < bytes_read) space_2 = i++;

    // заполняем url
    url.assign(buffer + space_1 + 1, buffer + space_2);

    // ищим конец строки
    while (buffer[i] != '\r' && buffer[i] != '\n' && i < bytes_read) ++i;

    // заполняем версию HTTP
    version.assign(buffer + space_2 + 1, buffer + i);

    // указываем начало новой строки
    if (buffer[i] == '\r') {
        if (i < bytes_read && (i + 1 < bytes_read)) i += 2;  
    } else {
        if (i < bytes_read) ++i;
    }

    // заполняем таблицу заголовков запроса (ключ-значение)
    while (i < bytes_read) {

        // начало ключа
        int key_start = i++;

        // ищем разделитель ":"
        while (buffer[i] != ':' && i < bytes_read) ++i;

        // конец ключа
        int key_stop = i++;

        // пропускаем пробел
        if (i < bytes_read) ++i;

        // начало значения
        int value_start = i;

        // ищим конец строки
        while (buffer[i] != '\r' && buffer[i] != '\n' && i < bytes_read) ++i;

        // заносим ключ-значение
        std::string key(buffer + key_start, buffer + key_stop);
        std::string value(buffer + value_start, buffer + i);
        headlines.emplace(std::move(key), std::move(value));

        // указываем начало новой строки
        if (buffer[i] == '\r') {
            if (i < bytes_read && (i + 1 < bytes_read)) i += 2;  
        } else {
            if (i < bytes_read) ++i;
        }

        // проверяем не закончились ли заголовки
        if (buffer[i] == '\r') {
            if (i < bytes_read) i += 2;
            // указываем начало тела запроса
            begin_request_body = i;
            break;
        } else if (buffer[i] == '\n') {
            if (i < bytes_read) ++i;
            // указываем начало тела запроса
            begin_request_body = i;
            break;
        }
    }

}

std::string Request::GET_headlines(const std::string& key) const {
    return headlines.at(key);
}

void Request::Show_RequestHeders() const {
    switch (method) {
        case Method::GET:    std::cout << "GET"; break;
        case Method::POST:   std::cout << "POST"; break;
        case Method::HEAD:   std::cout << "HEAD"; break;
        case Method::PUT:    std::cout << "PUT"; break;
        case Method::DELETE: std::cout << "DELETE"; break;
        case Method::OPTIONS:std::cout << "OPTIONS"; break;
        case Method::PATCH:  std::cout << "PATCH"; break;
        case Method::ERROR:  std::cout << "ERROR"; break;
    }
    std::cout << " " << url << " " << version << std::endl;
    for (const auto& pair : headlines) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    std::cout << std::endl;
}
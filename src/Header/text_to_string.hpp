// класс для чтения текстового файла в строку
#pragma once

#include <string>
#include <vector>
#include <sys/stat.h>

class TextToString {
    private:
        // индикатор ошибок, true - нет ошибок
        bool flag_error = true;
        // ошибки
        std::string str_error;
        // информация о файле
        struct stat file_stat;
        // прочитаный html
        std::string html_buffer;
        // массив строк html
        std::vector<std::string> vec_str;
        // чтение файла
        void read_file(const std::string & filename);
        // сборка всех строк в одну
        void vector_to_string();
    public:
        TextToString(const std::string & filename);
        ~TextToString() {};
        const bool GET_flag_error() const {return flag_error;};
        const std::string & GET_str_error() const {return str_error;}
        const struct stat & GET_file_stat() const {return file_stat;}
        const std::string & GET_file_buffer() const {return html_buffer;}
};
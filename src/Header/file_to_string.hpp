// класс для чтения файла в строку в бинарном формате
#pragma once

#include <sys/stat.h>
#include <string>
#include <fstream>

#include "logger.hpp"

class FileToString {
    private:
        // индикатор ошибок, true - нет ошибок
        bool flag_error = true;
        // информация о файле
        struct stat file_stat;
        // прочитаный фаил
        std::string file_buffer;
    public:
        FileToString(const std::string & filename);
        ~FileToString() {};
        const bool GET_flag_error() const {return flag_error;};
        const struct stat & GET_file_stat() const {return file_stat;}
        const std::string & GET_file_buffer() const {return file_buffer;}
};

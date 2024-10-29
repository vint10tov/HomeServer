// класс для чтения файла в строку в бинарном формате
#pragma once

#include <sys/stat.h>

#include <string>
#include <fstream>

class FileResponse {
    private:
        // индикатор ошибок, true - нет ошибок
        bool flag_error = true;
        // ошибки
        std::string str_error;
        // информация о файле
        struct stat file_stat;
        // прочитаный фаил
        std::string file_buffer;
    public:
        FileResponse(const std::string & filename);
        ~FileResponse() {};
        const bool GET_flag_error() const {return flag_error;};
        const std::string & GET_str_error() const {return str_error;}
        const struct stat & GET_file_stat() const {return file_stat;}
        const std::string & GET_file_buffer() const {return file_buffer;}
};

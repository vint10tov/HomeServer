#include <fstream>
#include <string>
#include <regex>
#include <iostream>

#include "text_to_string.hpp"
#include "logger.hpp"

TextToString::TextToString(const std::string& filename) {
    read_file(filename);
    vector_to_string();
}

void TextToString::read_file(const std::string& filename) {

    // Получаем информацию о файле
    if (stat(filename.c_str(), &file_stat) != 0) {
        str_error = "Ошибка при получении информации о файле: " + filename;
        Logger::error_log(str_error);
        flag_error = false;
        return;
    }

    // выражение для поиска #{}
    std::regex regexPattern(R"(\#\{(.*?)\})");

    std::ifstream file(filename);
    if (!file.is_open()) {
        str_error = "Не удается открыть файл: " + filename;
        Logger::error_log(str_error);
        flag_error = false;
        return;
    }
    while (std::getline(file, html_buffer)) {
        std::smatch match;
        if (std::regex_search(html_buffer, match, regexPattern)) {
            read_file(match[1].str());
        } else {
            vec_str.push_back(html_buffer);
        }
    }
    file.close();
    html_buffer = "";
}

void TextToString::vector_to_string() {
    for (auto & iter : vec_str) {
        html_buffer += iter += "\n";
    }
}
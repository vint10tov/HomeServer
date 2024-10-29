#include <fstream>
#include <string>
#include <regex>

#include "text_response.hpp"

bool TextResponse::read_file(const std::string& filename) {

    // Получаем информацию о файле
    if (stat(filename.c_str(), &file_stat) != 0) {
        str_error = "Ошибка при получении информации о файле: " + filename;
        flag_error = false;
        return true;
    }

    std::regex regexPattern(R"(\#\{(.*?)\})");

    std::ifstream file(filename);
    if (!file.is_open()) {
        str_error = "Не удается открыть файл: " + filename;
        flag_error = false;
        return false;
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
    return true;
}

bool TextResponse::vector_to_string() {
    for (auto & iter : vec_str) {
        html_buffer += iter += "\n";
    }
    return true;
}

TextResponse::TextResponse(const std::string& filename) {
    read_file(filename);
    vector_to_string();
}

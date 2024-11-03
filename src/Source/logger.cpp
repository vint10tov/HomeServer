# include <iostream>

#include "logger.hpp"

void Logger::debug_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << message << std::endl;
}

void Logger::info_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << INFO_green_begin << message << INFO_green_end << std::endl;
}

void Logger::warn_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << WARN_yellow_begin << message << WARN_yellow_end << std::endl;
}

void Logger::error_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << ERROR_red_begin << message << ERROR_red_end << std::endl;
}
# include <iostream>

#include "logger.hpp"

std::mutex Logger::mutex_log;

void Logger::debug_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << message << std::endl;
}

void Logger::info_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << GREEN << message << RESET << std::endl;
}

void Logger::warn_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << YELLOW << message << RESET << std::endl;
}

void Logger::error_log(std::string message) {
    std::lock_guard<std::mutex> lock(mutex_log); // Защита от многопоточного доступа
    std::cout << RED << message << RESET << std::endl;
}
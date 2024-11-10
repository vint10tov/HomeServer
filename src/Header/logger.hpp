#pragma once

#include <mutex>
#include <string>

// Коды цветов
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";

class Logger {
    public:
        //enum class LogType {DEBUG, INFO, WARN, ERROR};
        static void debug_log(std::string message);
        static void info_log(std::string message);
        static void warn_log(std::string message);
        static void error_log(std::string message);
    private:
        static std::mutex mutex_log;       // Мьютекс для защиты многопоточности
};
#pragma once

#include <mutex>
#include <string>

const char INFO_green_begin[] = "\032[1;32m";
const char INFO_green_end[] = "\032[0m";
const char WARN_yellow_begin[] = "\033[1;32m";
const char WARN_yellow_end[] = "\033[0m";
const char ERROR_red_begin[] = "\031[1;32m";
const char ERROR_red_end[] = "\031[0m";

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
// шаблонизатор для html
#pragma once

#include <string>

#include "uart_uno.hpp"
#include "smart_home.hpp"

const char ON_STAT[] = "ON";
const char OFF_STAT[] = "OFF";
const char ON_STAT_1[] = "1";
const char OFF_STAT_0[] = "0";
const char ACT[] = "ACT";
const char DEACT[] = "DEACT";
const char questions[] = "??";
const char PATTERN[] = R"(\#\[(.*?)\])";

class TemplateHTML {
    public:
        // функция получения обработанной строки
        static std::string replace_matches(std::string& input, SmartHome & smart_home);
        
    private:
        static bool is_number(const std::string& str);
};
// шаблонизатор для html
#pragma once

#include <string>

#include "uart_uno.hpp"

const char get_time[] = "gt/tm//;";
const char get_relay[] = "gt/rl/pn//;";
const char get_mod2_0[] = "gt/rl/md/=2/0//;";
const char get_mod2_1[] = "gt/rl/md/=2/1//;";
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
        static std::string replace_matches(std::string& input, UartUno * uartuno);
        
    private:
        static bool is_number(const std::string& str);
};
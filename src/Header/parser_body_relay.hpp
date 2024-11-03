// класс для преобразования тела запроса от браузера на /relay в строки для отправки на UNO
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>

#include "logger.hpp"

const uint HOUR = 60;
const char REPEAT_ON[] = "on";
const char RELAY_0[] = "r0";
const char RELAY_1[] = "r1";
const char RELAY_2[] = "r2";
const char RELAY_2_MOD_0[] = "r2m0";
const char RELAY_2_MOD_1[] = "r2m1";
const char POST_RELAY_0[] = "pt/rl/pn/=0/";
const char POST_RELAY_1[] = "pt/rl/pn/=1/";
const char POST_RELAY_2[] = "pt/rl/pn/=2/";
const char POST_RELAY_1_MOD_0_OFF[] = "pt/rl/md/of/=1/0/";
const char POST_RELAY_2_MOD_0_ACT[] = "pt/rl/md/ac/=2/0/";
const char POST_RELAY_2_MOD_1_ACT[] = "pt/rl/md/ac/=2/1/";
const char SLASH[] = "/";
const char SLASH_0_1[] = "/0/1//;";
const char POST_END[] = "//;";
const char POST_RELAY_2_MOD_0_ON[] = "pt/rl/md/on/=2/0/";
const char POST_RELAY_2_MOD_0_OFF[] = "pt/rl/md/off/=2/0/";
const char POST_RELAY_2_MOD_1_ON[] = "pt/rl/md/on/=2/1/";
const char POST_RELAY_2_MOD_1_OFF[] = "pt/rl/md/off/=2/1/";
const char POST_TIME_UNO[] = "pt/tm/=0/";
const char TIME_RELAY_0_MOD_0[] = "h.on.r2m0";
const char TIME_RELAY_0_MOD_1[] = "h.on.r2m1";
const char POST_RELAY_2_MOD_0_ACT_ON[] = "pt/rl/md/rt/=2/0/1//;";
const char POST_RELAY_2_MOD_1_ACT_ON[] = "pt/rl/md/rt/=2/1/1//;";
const char TIME[] = "hour";
const char _0_0[] = "/0/0//;";

class ParserBodyRelay {
public:
    ParserBodyRelay(std::string &body_relay);
    const std::vector<std::string> & GET_resul() {return resul;}
    // наличие строк для отправки
    bool string_out() {return count;}
private:
    
    // строки для отправки на UNO
    std::vector<std::string> resul;
    // количество строк для отправки
    uint count;
    // массив входящих строк
    std::vector<std::string> key;
    // массив входящих значений
    std::vector<int> value;

    // разделение строки на ключ значение
    void parse(const std::string &body_relay);
    // преобразование значения в int
    int parseValue(const std::string &v) const;
    // если только одно значение
    void count_1();
    // если больше одного значение
    void count_more_1();
    // функция получения времени сервера
    void time_server(uint & hour, uint & minute);
};
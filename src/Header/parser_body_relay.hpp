// класс для преобразования тела запроса от браузера на /relay в строки для отправки на UNO
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>

#include "request_from_server.hpp"
#include "logger.hpp"

const uint HOUR = 60;
const char REPEAT_ON[] = "on";
const char RELAY_0[] = "r0";
const char RELAY_1[] = "r1";
const char RELAY_2[] = "r2";
const char RELAY_2_MOD_0[] = "r2m0";
const char RELAY_2_MOD_1[] = "r2m1";
const char TIME_RELAY_0_MOD_0[] = "h.on.r2m0";
const char TIME_RELAY_0_MOD_1[] = "h.on.r2m1";
const char TIME[] = "hour";

class ParserBodyRelay {
public:
    ParserBodyRelay(std::string &body_relay);
    RequestFromServer & GET_request_from_server() {return rfs;}
    // наличие строк для отправки
    bool string_out() {return count;}
private:
    RequestFromServer rfs;
    // количество строк
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
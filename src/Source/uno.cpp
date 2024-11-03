#include "uno.hpp"
#include "logger.hpp"
#include "parser_uno.hpp"

UNO::UNO(UartUno* uartuno) {
    // если последовательный порт открыт
    if (uartuno->isOpen()) {
        std::string ping = uartuno->sending_string(get_ping);
        ParserUno pars_uno(ping);
        if (pars_uno.GET_error()) {
            Logger::info_log("UNO: ping = " + pars_uno.GET_value(0));
        } else {
            Logger::error_log("UNO: ping = нет связи");
        }
    } else {
        return;
    }
    // запрос всех параметров у UNO
    std::string time = uartuno->sending_string(get_time);
    ParserUno pars_time(time);
    std::string relay_stat = uartuno->sending_string(get_relay);
    ParserUno pars_relay(relay_stat);
    std::string mod2_0 = uartuno->sending_string(get_mod2_0);
    ParserUno pars_mod2_0(mod2_0);
    std::string mod2_1 = uartuno->sending_string(get_mod2_1);
    ParserUno pars_mod2_1(mod2_1);

    if (pars_time.GET_type() == ParserUno::Type::OK) {
        time_sec = pars_time.GET_value(0);
        time_min = pars_time.GET_value(1);
        time_hour = pars_time.GET_value(2);
        time_day = pars_time.GET_value(3);
        time_moun = pars_time.GET_value(4);
        time_year = pars_time.GET_value(5);
    } else {
        Logger::warn_log("UNO: time = ERROR");
    }
    if (pars_relay.GET_type() == ParserUno::Type::OK) {
        relay[0].status = pars_relay.GET_value(1);
        relay[1].status = pars_relay.GET_value(3);
        relay[2].status = pars_relay.GET_value(5);
    } else {
        Logger::warn_log("UNO: relay = ERROR");
    }
    if (pars_mod2_0.GET_type() == ParserUno::Type::OK) {
        relay[2].mod[0].act_on = pars_mod2_0.GET_value(0);
        relay[2].mod[0].act_off = pars_mod2_0.GET_value(1);
        relay[2].mod[0].repeat = pars_mod2_0.GET_value(2);
        relay[2].mod[0].mod_hour_on = pars_mod2_0.GET_value(3);
        relay[2].mod[0].mod_min_on = pars_mod2_0.GET_value(4);
        relay[2].mod[0].mod_hour_off = pars_mod2_0.GET_value(5);
        relay[2].mod[0].mod_min_off = pars_mod2_0.GET_value(6);
    } else {
        Logger::warn_log("UNO: mod/2/0 = ERROR");
    }
    if (pars_mod2_1.GET_type() == ParserUno::Type::OK) {
        relay[2].mod[1].act_on = pars_mod2_1.GET_value(0);
        relay[2].mod[1].act_off = pars_mod2_1.GET_value(1);
        relay[2].mod[1].repeat = pars_mod2_1.GET_value(2);
        relay[2].mod[1].mod_hour_on = pars_mod2_1.GET_value(3);
        relay[2].mod[1].mod_min_on = pars_mod2_1.GET_value(4);
        relay[2].mod[1].mod_hour_off = pars_mod2_1.GET_value(5);
        relay[2].mod[1].mod_min_off = pars_mod2_1.GET_value(6);
    } else {
        Logger::warn_log("UNO: mod/2/1 = ERROR");
    }
}

std::string UNO::GET_time() const {
    // Формирование результата
    return (addLeadingZero(time_hour) + ":" +
              addLeadingZero(time_min) + ":" +
              addLeadingZero(time_sec) + " " +
              std::to_string(time_day) + " " +
              months[time_moun - 1] + " " + // Индексы массива начинаются с 0
              std::to_string(time_year));
}

std::string UNO::GET_mod_on(uint r, uint m) const {
    return (addLeadingZero(relay[r].mod[m].mod_hour_on) + ":" +
            addLeadingZero(relay[r].mod[m].mod_min_on));
}

std::string UNO::GET_mod_off(uint r, uint m) const {
    return (addLeadingZero(relay[r].mod[m].mod_hour_off) + ":" +
            addLeadingZero(relay[r].mod[m].mod_min_off));
}

// Функция для добавления ведущего нуля
std::string UNO::addLeadingZero(int value) const{
    return (value < 10 ? "0" : "") + std::to_string(value);
}
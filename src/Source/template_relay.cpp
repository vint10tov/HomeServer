#include <regex>

#include "parser_uno.hpp"
#include "template_relay.hpp"

std::string replace_matches(std::string& input, UartUno* uartuno) {

    std::string time = uartuno->sending_string(get_time);
    ParserUno pars_time(time);
    std::string relay = uartuno->sending_string(get_relay);
    ParserUno pars_relay(relay);
    std::string mod2_0 = uartuno->sending_string(get_mod2_0);
    ParserUno pars_mod2_0(mod2_0);
    std::string mod2_1 = uartuno->sending_string(get_mod2_1);
    ParserUno pars_mod2_1(mod2_1);
    
    std::regex pattern(R"(\#\[(.*?)\])");
    std::string result = input;
    std::smatch match;

    // Используем цикл для поиска и замены всех вхождений
    while (std::regex_search(result, match, pattern)) {
        // Получаем текст внутри #[].
        std::string inner = match[1].str();
        
        // Здесь добавляем логику замены
        std::string replacement = "";
        if (is_number(inner)) { // Строка содержит число
            switch (atoi(inner.c_str())) {
            case 0:
                if (pars_time.GET_type() == ParserUno::Type::OK) {
                    replacement = std::to_string(pars_time.GET_value(2)) + ":" +
                                  std::to_string(pars_time.GET_value(1)) + ":" +
                                  std::to_string(pars_time.GET_value(0)) + "  " +
                                  std::to_string(pars_time.GET_value(3)) + "/" +
                                  std::to_string(pars_time.GET_value(4)) + "/" +
                                  std::to_string(pars_time.GET_value(5));
                }
                break;
            case 1:
                if (pars_mod2_0.GET_type() == ParserUno::Type::OK) {
                    replacement = std::to_string(pars_mod2_0.GET_value(3)) + ":" +
                                  std::to_string(pars_mod2_0.GET_value(4));
                }
                break;
            case 2:
                if (pars_mod2_0.GET_type() == ParserUno::Type::OK) {
                    replacement = std::to_string(pars_mod2_0.GET_value(5)) + ":" +
                                  std::to_string(pars_mod2_0.GET_value(6));
                }
                break;
            case 3:
                if (pars_mod2_1.GET_type() == ParserUno::Type::OK) {
                    replacement = std::to_string(pars_mod2_1.GET_value(3)) + ":" +
                                  std::to_string(pars_mod2_1.GET_value(4));
                }
                break;
            case 4:
                if (pars_mod2_1.GET_type() == ParserUno::Type::OK) {
                    replacement = std::to_string(pars_mod2_1.GET_value(5)) + ":" +
                                  std::to_string(pars_mod2_1.GET_value(6));
                }
                break;
            case 5:
                if (pars_mod2_0.GET_type() == ParserUno::Type::OK) {
                    if (pars_mod2_0.GET_value(0))
                        replacement = "ON";
                    else
                        replacement = "OFF";
                }
                break;
            case 6:
                if (pars_mod2_1.GET_type() == ParserUno::Type::OK) {
                    if (pars_mod2_1.GET_value(0))
                        replacement = "ON";
                    else
                        replacement = "OFF";
                }
                break;
            case 7:
                if (pars_relay.GET_type() == ParserUno::Type::OK) {
                    if (pars_relay.GET_value(1))
                        replacement = "0";
                    else
                        replacement = "1";
                }
                break;
            case 8:
                if (pars_relay.GET_type() == ParserUno::Type::OK) {
                    if (pars_relay.GET_value(1))
                        replacement = "OFF";
                    else
                        replacement = "ON";
                }
                break;
            case 9:
                if (pars_relay.GET_type() == ParserUno::Type::OK) {
                    if (pars_relay.GET_value(3))
                        replacement = "0";
                    else
                        replacement = "1";
                }
                break;
            case 10:
                if (pars_relay.GET_type() == ParserUno::Type::OK) {
                    if (pars_relay.GET_value(3))
                        replacement = "OFF";
                    else
                        replacement = "ON";
                }
                break;
            case 11:
                if (pars_relay.GET_type() == ParserUno::Type::OK) {
                    if (pars_relay.GET_value(5))
                        replacement = "0";
                    else
                        replacement = "1";
                }
                break;
            case 12:
                if (pars_relay.GET_type() == ParserUno::Type::OK) {
                    if (pars_relay.GET_value(5))
                        replacement = "OFF";
                    else
                        replacement = "ON";
                }
                break;
            case 13:
                if (pars_mod2_0.GET_type() == ParserUno::Type::OK) {
                    if (pars_mod2_0.GET_value(0))
                        replacement = "0";
                    else
                        replacement = "1";
                }
                break;
            case 14:
                if (pars_mod2_0.GET_type() == ParserUno::Type::OK) {
                    if (pars_mod2_0.GET_value(0))
                        replacement = "DEACT";
                    else
                        replacement = "ACT";
                }
                break;
            case 15:
                if (pars_mod2_1.GET_type() == ParserUno::Type::OK) {
                    if (pars_mod2_1.GET_value(0))
                        replacement = "0";
                    else
                        replacement = "1";
                }
                break;
            case 16:
                if (pars_mod2_1.GET_type() == ParserUno::Type::OK) {
                    if (pars_mod2_1.GET_value(0))
                        replacement = "DEACT";
                    else
                        replacement = "ACT";
                }
                break;
            default:
                break;
            }
        } else { // Строка содержит не только число
            //
        }
        // Заменяем найденное вхождение на новое значение
        result.replace(match.position(0), match.length(0), replacement);
    }
    return result;
}
    
bool is_number(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
#include <regex>

#include "parser_uno.hpp"
#include "template_relay.hpp"
#include "uno.hpp"
#include "logger.hpp"

std::string TemplateHTML::replace_matches(std::string& input, UartUno * uartuno) {
    UNO uno(uartuno);
    
    // #[]
    std::regex pattern(PATTERN);
    std::string result = input;
    std::smatch match;

    // Используем цикл для поиска и замены всех вхождений
    while (std::regex_search(result, match, pattern)) {
        // Получаем текст внутри #[].
        std::string inner = match[1].str();
        
        // Здесь добавляем логику замены
        std::string replacement = questions;
        if (is_number(inner)) { // Строка содержит число

            size_t num = atoi(inner.c_str());

            std::vector<std::function<std::string()>> actions = {
                [&]() { return uno.GET_time(); },                                     // 0
                [&]() { return uno.GET_mod_on(2, 0); },                               // 1
                [&]() { return uno.GET_mod_off(2, 0); },                              // 2
                [&]() { return uno.GET_mod_on(2, 1); },                               // 3
                [&]() { return uno.GET_mod_off(2, 1); },                              // 4
                [&]() { return uno.GET_mod_repeat(2, 0) ? ON_STAT : OFF_STAT; },      // 5
                [&]() { return uno.GET_mod_repeat(2, 1) ? ON_STAT : OFF_STAT; },      // 6
                [&]() { return uno.GET_status_relay(0) ? OFF_STAT_0 : ON_STAT_1; },   // 7
                [&]() { return uno.GET_status_relay(0) ? OFF_STAT : ON_STAT; },       // 8
                [&]() { return uno.GET_status_relay(1) ? OFF_STAT_0 : ON_STAT_1; },   // 9
                [&]() { return uno.GET_status_relay(1) ? OFF_STAT : ON_STAT; },       // 10
                [&]() { return uno.GET_status_relay(2) ? OFF_STAT_0 : ON_STAT_1; },   // 11
                [&]() { return uno.GET_status_relay(2) ? OFF_STAT : ON_STAT; },       // 12
                [&]() { return uno.GET_mod_act_on(2, 0) ? OFF_STAT_0 : ON_STAT_1; },  // 13
                [&]() { return uno.GET_mod_act_on(2, 0) ? DEACT : ACT; },             // 14
                [&]() { return uno.GET_mod_act_on(2, 1) ? OFF_STAT_0 : ON_STAT_1; },  // 15
                [&]() { return uno.GET_mod_act_on(2, 1) ? DEACT : ACT; }              // 16
            };

            if (num >= 0 && num < actions.size()) {
                replacement = actions[num]();
            } else {
                replacement = questions;
                Logger::warn_log("TemplateHTML: необрабатываемое значение");
            }
        } else { // Строка содержит не только число
            Logger::warn_log("TemplateHTML: нечисловое значение");
        }
        // Заменяем найденное вхождение на новое значение
        result.replace(match.position(0), match.length(0), replacement);
    }
    return result;
}
    
bool TemplateHTML::is_number(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
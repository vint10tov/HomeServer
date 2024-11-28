#include <regex>

#include "template_relay.hpp"
#include "smart_home.hpp"
#include "request_from_server.hpp"
#include "logger.hpp"

std::string TemplateHTML::replace_matches(std::string& input, SmartHome & smart_home) {
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
                [&]() { return smart_home.showTime(); },                                     // 0
                [&]() { return smart_home.showTimeModOn(2, 0); },                            // 1
                [&]() { return smart_home.showTimeModOff(2, 0); },                           // 2
                [&]() { return smart_home.showTimeModOn(2, 1); },                            // 3
                [&]() { return smart_home.showTimeModOff(2, 1); },                           // 4
                [&]() { return "ON"; },                                                      // 5
                [&]() { return "ON"; },                                                      // 6
                [&]() { return smart_home.GET_status_relay(0) ? OFF_STAT_0 : ON_STAT_1; },   // 7
                [&]() { return smart_home.GET_status_relay(0) ? OFF_STAT : ON_STAT; },       // 8
                [&]() { return smart_home.GET_status_relay(1) ? OFF_STAT_0 : ON_STAT_1; },   // 9
                [&]() { return smart_home.GET_status_relay(1) ? OFF_STAT : ON_STAT; },       // 10
                [&]() { return smart_home.GET_status_relay(2) ? OFF_STAT_0 : ON_STAT_1; },   // 11
                [&]() { return smart_home.GET_status_relay(2) ? OFF_STAT : ON_STAT; },       // 12
                [&]() { return smart_home.GET_status_mod(2, 0) ? OFF_STAT_0 : ON_STAT_1; },  // 13
                [&]() { return smart_home.GET_status_mod(2, 0) ? DEACT : ACT; },             // 14
                [&]() { return smart_home.GET_status_mod(2, 1) ? OFF_STAT_0 : ON_STAT_1; },  // 15
                [&]() { return smart_home.GET_status_mod(2, 1) ? DEACT : ACT; },             // 16
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
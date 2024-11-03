#include "parser_body_relay.hpp"

ParserBodyRelay::ParserBodyRelay(std::string &body_relay) {
    parse(body_relay);
    count = key.size();
    if (count == 0) {
        Logger::warn_log("ParserBodyRelay: нет строк для отправки на UNO");
    } else if (count == 1) {
        count_1();
    } else {
        count_more_1();
    }
}

void ParserBodyRelay::parse(const std::string &body_relay) {
    std::istringstream stream(body_relay);
    std::string pair;

    while (std::getline(stream, pair, '&')) {
        size_t pos = pair.find('=');
        std::string k;
        std::string v;

        if (pos != std::string::npos) {
            k = pair.substr(0, pos);
            if ((pos + 1) != '\0')
                v = pair.substr(pos + 1);
            else
                v = "";
        } else {
            k = pair; // Если '=' нет, то ключ - это вся строка
            v = "";   // Значение пустое
        }

        // Добавляем ключ и преобразуем значение
        key.push_back(k);
        value.push_back(parseValue(v));
    }
}

int ParserBodyRelay::parseValue(const std::string &v) const {
    if (v.empty()) {
        return 0; // Пустое значение становится 0
    } else if (v == REPEAT_ON) {
        return 1; // Значение 'on' становится 1
    } else {
        try {
            return std::stoi(v); // Преобразуем строку в число
        } catch (const std::invalid_argument &) {
            return 0; // При ошибке - возвращаем 0
        } catch (const std::out_of_range &) {
            return 0; // Если число выходит за пределы, возвращаем 0
        }
    }
}

// если только одно значение
void ParserBodyRelay::count_1() {
    if (key[0] == RELAY_0) { // вкл-выкл реле 0
        resul.push_back(POST_RELAY_0 + std::to_string(value[0]) + POST_END);
    } else if (key[0] == RELAY_1) { // реле 1
        if (value[0] < 2) { // вкл-выкл реле 1
            resul.push_back(POST_RELAY_1 + std::to_string(value[0]) + POST_END);
        } else { // включение реле 1 на время в минутах
            uint hour;
            uint minute;
            time_server(hour, minute);
            hour += (value[0] / HOUR);
            minute += (value[0] % HOUR);
            resul.push_back(POST_RELAY_1_MOD_0_OFF + std::to_string(minute) +
                            SLASH + std::to_string(hour) + SLASH_0_1);
        }
    } else if (key[0] == RELAY_2) { // вкл-выкл реле 2
        resul.push_back(POST_RELAY_2 + std::to_string(value[0]) + POST_END);
    } else if (key[0] == RELAY_2_MOD_0) { // активация-деактивация реле 1 режим 0
        resul.push_back(POST_RELAY_2_MOD_0_ACT + std::to_string(value[0]) + POST_END);
    } else if (key[0] == RELAY_2_MOD_1) { // активация-деактивация реле 1 режим 1
        resul.push_back(POST_RELAY_2_MOD_1_ACT + std::to_string(value[0]) + POST_END);
    } else {
        Logger::warn_log("ParserBodyRelay: строка не корректная");
        count = 0;
    }
}

// если больше одного значение
void ParserBodyRelay::count_more_1() {
    if (key[0] == TIME_RELAY_0_MOD_0 && count > 3) { // установка времени реле 2 мод 0
        resul.push_back(POST_RELAY_2_MOD_0_ON + std::to_string(value[1]) +
                        SLASH + std::to_string(value[0]) + _0_0);
        resul.push_back(POST_RELAY_2_MOD_0_OFF + std::to_string(value[3]) +
                        SLASH + std::to_string(value[2]) + _0_0);
        if (count == 5) { // активация режима 0
            resul.push_back(POST_RELAY_2_MOD_0_ACT_ON);
        }
    } else if (key[0] == TIME_RELAY_0_MOD_1 && count > 3) { // установка времени реле 2 мод 1
        resul.push_back(POST_RELAY_2_MOD_1_ON + std::to_string(value[1]) +
                        SLASH + std::to_string(value[0]) + _0_0);
        resul.push_back(POST_RELAY_2_MOD_1_OFF + std::to_string(value[3]) +
                        SLASH + std::to_string(value[2]) + _0_0);
        if (count == 5) { // активация режима 1
            resul.push_back(POST_RELAY_2_MOD_1_ACT_ON);
        }
    } else if (key[0] == TIME && count == 5) { // установка времени UNO
        resul.push_back(POST_TIME_UNO + std::to_string(value[1]) + "/" + 
                        std::to_string(value[0]) + SLASH + std::to_string(value[2]) + SLASH + 
                        std::to_string(value[3]) + SLASH + std::to_string(value[4]) + POST_END);
    } else {
        Logger::warn_log("ParserBodyRelay: строка не корректная");
        count = 0;
    }
}

// функция получения времени сервера
void ParserBodyRelay::time_server(uint & hour, uint & minute) {
    // Получаем текущее время с помощью chrono
    auto now = std::chrono::system_clock::now();
    // Преобразуем текущее время в time_t
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    // Преобразуем time_t в структуру tm для получения часов и минут
    std::tm* now_tm = std::localtime(&now_c);
    // Получаем часы и минуты
    hour = now_tm->tm_hour;
    minute = now_tm->tm_min;
}
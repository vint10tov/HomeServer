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
        rfs.SET_PIN(0, (bool)value[0]);
    } else if (key[0] == RELAY_1) { // реле 1
        if (value[0] < 2) { // вкл-выкл реле 1
            rfs.SET_PIN(1, (bool)value[0]);
        } else { // включение реле 1 на время в минутах
            uint hour;
            uint minute;
            time_server(hour, minute);
            uint temp = hour * HOUR + minute + value[0];
            rfs.SET_PIN_MIN(1, 0, (uint8_t)(temp / HOUR), (uint8_t)(temp % HOUR));
        }
    } else if (key[0] == RELAY_2) { // вкл-выкл реле 2
        rfs.SET_PIN(2, (bool)value[0]);
    } else if (key[0] == RELAY_2_MOD_0) { // активация-деактивация реле 2 режим 0
        rfs.SET_MOD(2, 0, value[0]);
    } else if (key[0] == RELAY_2_MOD_1) { // активация-деактивация реле 2 режим 1
        rfs.SET_MOD(2, 1, value[0]);
    } else {
        Logger::warn_log("ParserBodyRelay: строка не корректная (1)" + key[0]);
        count = 0;
    }
}

// если больше одного значение
void ParserBodyRelay::count_more_1() {
    if (key[0] == TIME_RELAY_0_MOD_0 && count > 3) { // установка времени реле 2 мод 0
        rfs.SET_MOD_T(2, 0, value[1], value[0], value[3], value[2]);
    } else if (key[0] == TIME_RELAY_0_MOD_1 && count > 3) { // установка времени реле 2 мод 1
        rfs.SET_MOD_T(2, 1, value[1], value[0], value[3], value[2]);
    } else if (key[0] == TIME && count == 5) { // установка времени UNO
        rfs.SET_TIME(value[1], value[0], value[2], value[3], value[4]);
    } else {
        Logger::warn_log("ParserBodyRelay: строка не корректная (2)" + key[0]);
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
#include "parser_body_relay.hpp"

ParserBodyRelay::ParserBodyRelay(std::string &req_body) {
    parse(req_body);
    count = key.size();
    if (count == 0) {
        resul.push_back("");
    } else if (count == 1) {
        if (key[0] == "r0") {
            resul.push_back("pt/rl/pn/=0/" + std::to_string(value[0]) + "//;");
        } else if (key[0] == "r1") {
            if (value[0] < 2) {
                resul.push_back("pt/rl/pn/=1/" + std::to_string(value[0]) + "//;");
            } else {
                // Получаем текущее время с помощью chrono
                auto now = std::chrono::system_clock::now();
                // Преобразуем текущее время в time_t
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                // Преобразуем time_t в структуру tm для получения часов и минут
                std::tm* now_tm = std::localtime(&now_c);
                // Получаем часы и минуты
                int hour = now_tm->tm_hour;
                int minute = now_tm->tm_min;
                hour += (value[0] / 60);
                minute += (value[0] % 60);
                resul.push_back("pt/rl/md/of/=1/0/" + std::to_string(minute) +
                                "/" + std::to_string(hour) + "/0/1//;");
            }
        } else if (key[0] == "r2") {
            resul.push_back("pt/rl/pn/=2/" + std::to_string(value[0]) + "//;");
        } else if (key[0] == "r2m0") {
            resul.push_back("pt/rl/md/ac/=2/0/" + std::to_string(value[0]) + "//;");
        } else if (key[0] == "r2m1") {
            resul.push_back("pt/rl/md/ac/=2/1/" + std::to_string(value[0]) + "//;");
        } else {
            resul.push_back("");
        }
    } else {
        if (key[0] == "h.on.r2m0" && count > 3) {
            resul.push_back("pt/rl/md/on/=2/0/" + std::to_string(value[1]) +
                            "/" + std::to_string(value[0]) + "/0/0//;");
            resul.push_back("pt/rl/md/on/=2/0/" + std::to_string(value[3]) +
                            "/" + std::to_string(value[2]) + "/0/0//;");
            if (count == 5) {
                resul.push_back("pt/rl/md/rt/=2/0/1//;");
            }
        } else if (key[0] == "h.on.r2m1" && count > 3) {
            resul.push_back("pt/rl/md/on/=2/1/" + std::to_string(value[1]) +
                            "/" + std::to_string(value[0]) + "/0/0//;");
            resul.push_back("pt/rl/md/on/=2/1/" + std::to_string(value[3]) +
                            "/" + std::to_string(value[2]) + "/0/0//;");
            if (count == 5) {
                resul.push_back("pt/rl/md/rt/=2/1/1//;");
            }
        } else if (key[0] == "hour" && count == 5) {
            resul.push_back("pt/tm/=0/" + std::to_string(value[1]) + "/" + 
                            std::to_string(value[0]) + "/" + std::to_string(value[2]) + "/" + 
                            std::to_string(value[3]) + "/" + std::to_string(value[4]) + "//;");
        } else {
            resul[0] = "";
        }
    }
}

void ParserBodyRelay::parse(const std::string &req_body) {
    std::istringstream stream(req_body);
    std::string pair;

    while (std::getline(stream, pair, '&')) {
        size_t pos = pair.find('=');
        std::string k;
        std::string v;

        if (pos != std::string::npos) {
            k = pair.substr(0, pos);
            v = pair.substr(pos + 1);
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
    } else if (v == "on") {
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
#include "request_from_server.hpp"
#include "logger.hpp"

void RequestFromServer::SET_PING() {
    flag = 0;
}

void RequestFromServer::SET_PIN(uint8_t relay, bool on) {
    this->relay = relay;
    if (on) {
        flag = 1;
    } else {
        flag = 3;
    }
}

void RequestFromServer::SET_PIN_MIN(uint8_t relay, uint8_t mod, uint8_t hour, uint8_t min) {
    this->relay = relay;
    this->mod = mod;
    hour_off = hour;
    min_off = min;
    flag = 2;
}

void RequestFromServer::SET_MOD(uint8_t relay, uint8_t mod, bool on) {
    this->relay = relay;
    this->mod = mod;
    if (on) {
        flag = 4;
    } else {
        flag = 5;
    }
}

void RequestFromServer::SET_MOD_T(uint8_t relay, uint8_t mod, uint8_t on_min,
                                  uint8_t on_hour, uint8_t off_min, uint8_t off_hour) {
    this->relay = relay;
    this->mod = mod;
    min_on = on_min;
    hour_on = on_hour;
    min_off = off_min;
    hour_off = off_hour;
    flag = 6;
}

void RequestFromServer::SET_TIME(uint8_t min, uint8_t hour,
                      uint8_t day, uint8_t month, uint8_t year) {
    min_on = min;
    hour_on = hour;
    this->day = day;
    this->month = month;
    this->year = year;
    flag = 7;
}

bool RequestFromServer::serialize(uint8_t * buffer, uint8_t size_buffer) const {
    if (size_buffer < 10) { // Минимальный размер буфера для сериализации всех полей
        Logger::error_log("RequestFromServer: Невозможно сериализовать данные");
        return false;
    }
    buffer[0] = flag;
    buffer[1] = relay;
    buffer[2] = mod;
    buffer[3] = min_on;
    buffer[4] = hour_on;
    buffer[5] = min_off;
    buffer[6] = hour_off;
    buffer[7] = day;
    buffer[8] = month;
    buffer[9] = year;
    return true;
}

std::string RequestFromServer::show_request() const {
    std::string resul;

    switch (flag) {
    case 0:
        resul = "RequestFromServer: PING\n";
        break;
    case 1:
        resul = "RequestFromServer: PIN_ON\n";
        break;
    case 2:
        resul = "RequestFromServer: PIN_ON_MIN\n";
        break;
    case 3:
        resul = "RequestFromServer: PIN_OFF\n";
        break;
    case 4:
        resul = "RequestFromServer: MOD_ON\n";
        break;
    case 5:
        resul = "RequestFromServer: MOD_OFF\n";
        break;
    case 6:
        resul = "RequestFromServer: MOD_T\n";
        break;
    case 7:
        resul = "RequestFromServer: TIME\n";
        break;
    default:
        resul = "RequestFromServer: ERROR\n";
        break;
    }

    resul += "RequestFromServer: Relau: ";
    resul += std::to_string(relay);
    resul += " Mod: ";
    resul += std::to_string(mod);
    resul += "\n";
    resul += "RequestFromServer: min_on: ";
    resul += std::to_string(min_on);
    resul += " hour_on: ";
    resul += std::to_string(hour_on);
    resul += " min_off: ";
    resul += std::to_string(min_off);
    resul += " hour_off: ";
    resul += std::to_string(hour_off);
    resul += "\n";
    resul += "RequestFromServer: day: ";
    resul += std::to_string(day);
    resul += " month: ";
    resul += std::to_string(month);
    resul += " year: ";
    resul += std::to_string(year);

    return resul;
}
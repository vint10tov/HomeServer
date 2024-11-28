#include "request_from_server.hpp"

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

void RequestFromServer::SET_PIN_MIN(uint8_t relay, uint8_t mod, uint8_t min) {
    this->relay = relay;
    this->mod = mod;
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
        return false;       // Невозможно сериализовать данные с недостаточным размером буфера
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
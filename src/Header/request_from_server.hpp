#pragma once

#include <cstdint>

enum class RequestType {PING, PIN_ON, PIN_ON_MIN, PIN_OFF,
                        MOD_ON, MOD_OFF, MOD_T, TIME};

class RequestFromServer {
    private:
        uint8_t flag;
        uint8_t relay;
        uint8_t mod;
        uint8_t min_on;
        uint8_t hour_on;
        uint8_t min_off;
        uint8_t hour_off;
        uint8_t day;
        uint8_t month;
        uint8_t year;
    public:
        RequestFromServer() {}
        void SET_PING();
        void SET_PIN(uint8_t relay, bool on);
        void SET_PIN_MIN(uint8_t relay, uint8_t mod, uint8_t min);
        void SET_MOD(uint8_t relay, uint8_t mod, bool on);
        void SET_MOD_T(uint8_t relay, uint8_t mod, uint8_t on_min, uint8_t on_hour,
                                                   uint8_t off_min, uint8_t off_hour);
        void SET_TIME(uint8_t min, uint8_t hour,
                      uint8_t day, uint8_t month, uint8_t year);
        bool serialize(uint8_t * buffer, uint8_t size_buffer) const;
};
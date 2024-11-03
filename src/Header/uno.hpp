// класс для хранения в памяти состояния UNO

#include "uart_uno.hpp"

const char get_ping[] = "gt/pg//;";
const char get_time[] = "gt/tm//;";
const char get_relay[] = "gt/rl/pn//;";
const char get_mod2_0[] = "gt/rl/md/=2/0//;";
const char get_mod2_1[] = "gt/rl/md/=2/1//;";
const char RELAY_URL[] = "/relay";
const uint RELAY_NUM = 3;
const uint RELAY_MOD = 2;

// Массив месяцев
const std::string months[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

class UNO{
    public:
        struct Mod
        {
            bool act_on = false;
            bool act_off = false;
            bool repeat = false;
            uint mod_min_on = 0;
            uint mod_hour_on = 0;
            uint mod_min_off = 0;
            uint mod_hour_off = 0;
        };
        struct Relay {
            bool status = false;
            Mod mod[RELAY_MOD];
        };
        
        UNO(UartUno* uartuno);
        std::string GET_time() const;
        std::string GET_mod_on(uint r, uint m) const;
        std::string GET_mod_off(uint r, uint m) const;
        bool GET_status_relay(uint r) const {return relay[r].status;}
        bool GET_mod_act_on(uint r, uint m) const {return relay[r].mod[m].act_on;}
        bool GET_mod_act_off(uint r, uint m) const {return relay[r].mod[m].act_off;}
        bool GET_mod_repeat(uint r, uint m) const {return relay[r].mod[m].repeat;}
    private:
        uint time_sec = 0;
        uint time_min = 0;
        uint time_hour = 0;
        uint time_day = 0;
        uint time_moun = 0;
        uint time_year = 0;
        Relay relay[RELAY_NUM]; // true включено
        // Функция для добавления ведущего нуля
        std::string addLeadingZero(int value) const;
};
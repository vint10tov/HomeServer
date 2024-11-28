#pragma once

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdint>

class UartUno final {
    private:
        static UartUno* instance;          // Указатель на единственный экземпляр
        static std::mutex mutex_uno;       // Мьютекс для защиты многопоточности
        int fd = -1;
        const char port_name[13] = "/dev/ttyUSB0";

        // Закрытый конструктор для предотвращения создания экземпляров
        UartUno();
        // Закрытый деструктор
        ~UartUno();
    public:
        // Метод для получения единственного экземпляра класса
        static UartUno* getInstance();

        // Метод для отправки строки в порт и чтения строки из порта
        bool sending_string(uint8_t * buffer_in, uint8_t * buffer_out, size_t size_buffer);

        // Метод для проверки, открыт ли порт
        bool isOpen() const;
};
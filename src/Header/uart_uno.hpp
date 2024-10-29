#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <mutex>

class UartUno {
    private:
        static UartUno* instance;          // Указатель на единственный экземпляр
        static std::mutex mutex_uno;       // Мьютекс для защиты многопоточности
        const char *portname;              // последовательный порт
        int fd = -1;
        char buffer[256];

        // Закрытый конструктор для предотвращения создания экземпляров
        UartUno(const char* & port_name);
        // Закрытый деструктор
        ~UartUno();
    public:
        // Метод для получения единственного экземпляра класса
        static UartUno* getInstance(const char* port_name);

        // Метод для чтения строки из порта
        std::string read_string();
        // Метод для отправки строки в порт
        void sending_string(const std::string & str);

        // Метод для проверки, открыт ли порт
        bool isOpen() const;
};
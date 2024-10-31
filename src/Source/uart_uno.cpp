#include "uart_uno.hpp"

// при создании указывается последовательный порт наприсер "/dev/ttyUSB0"
UartUno::UartUno(const char* & port_name) {
    const char *portname = port_name;
    fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        return;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // Устанавливаем параметры порта
    cfsetispeed(&options, B9600); // Скорость приема
    cfsetospeed(&options, B9600); // Скорость передачи

    options.c_cflag |= (CLOCAL | CREAD); // Игнорируем режим управления модемом, включаем прием
    options.c_cflag &= ~PARENB; // Без контроля четности
    options.c_cflag &= ~CSTOPB; // Один стоп-бит
    options.c_cflag &= ~CSIZE; // Очистить маску размера бита
    options.c_cflag |= CS8; // Установить 8 бит данных
    options.c_cflag &= ~CRTSCTS; // Без аппаратного управления

    // Устанавливаем опции терминала
    options.c_lflag &= ~(ICANON | ECHO); // Режим неканонической линии, выключаем эхо
    options.c_oflag &= ~OPOST; // Выключаем постобработку вывода

    // Сохраняем настройки
    tcsetattr(fd, TCSANOW, &options);
}

// Закрытый деструктор
UartUno::~UartUno() {
    if (isOpen()) {
        // Закрываем порт
        close(fd);
    }
}

// Метод для получения единственного экземпляра класса
UartUno* UartUno::getInstance(const char* port_name) {
    std::lock_guard<std::mutex> lock(mutex_uno); // Защита от многопоточного доступа
    if (instance == nullptr) {
        instance = new UartUno(port_name);
    }
    return instance;
}

// Метод для проверки, открыт ли порт
bool UartUno::isOpen() const {
    if (fd != -1)
        return true;
    else
        return false;
}

// Метод для отправки строки в порт и чтения строки из порта
std::string UartUno::sending_string(const std::string & str) {
    std::lock_guard<std::mutex> lock(mutex_uno); // Защита от многопоточного доступа
    if (isOpen()) {
        // Отправляем данные на Arduino
        write(fd, str.c_str(), str.size());
        // Ждем немного перед чтением (можно настроить)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Чтение ответа от Arduino
        int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Завершаем строку
        }
        return buffer;
    } else {
        return "ERROR UART";
    }
}
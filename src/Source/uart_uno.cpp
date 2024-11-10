#include "uart_uno.hpp"
#include "logger.hpp"

std::mutex UartUno::mutex_uno;

// при создании указывается последовательный порт например "/dev/ttyUSB0"
UartUno::UartUno() {
    fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        Logger::error_log("UartUno: ошибка открытия порта");
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
    Logger::info_log("UartUno: Последовательный порт открыт");
}

// Закрытый деструктор
UartUno::~UartUno() {
    if (isOpen()) {
        // Закрываем порт
        close(fd);
        delete instance;
        Logger::info_log("UartUno: Последовательный порт закрыт");
    }
}

// Метод для получения единственного экземпляра класса
UartUno* UartUno::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_uno); // Защита от многопоточного доступа
    if (instance == nullptr) {
        instance = new UartUno();
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
        ssize_t result = write(fd, str.c_str(), str.size());
        if (result == -1) {
            // Обработка ошибки
            Logger::error_log("UartUno: Write error");
        }
        // Ждем немного перед чтением (можно настроить)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Чтение ответа от Arduino
        int bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Завершаем строку
        }
        return buffer;
    } else {
        return "";
    }
}
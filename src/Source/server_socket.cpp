#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <thread>

#include "server_socket.hpp"
#include "logger.hpp"

// создание серверного сокета
ServerSocket::ServerSocket() {
    uint count = 10;
    while (count > 0) {
        descriptor_server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (descriptor_server_socket < 0) {
            Logger::error_log("Socket: Не удалось создать сокет.");
            std::this_thread::sleep_for(std::chrono::seconds(40)); // Ждем 40 секунд
            --count;
            continue;
        }

        // хранения адресной информации для IPv4
        sockaddr_in serverAddr{};
        // протоколы IPv4
        serverAddr.sin_family = AF_INET; 
        // сервер будет принимать соединения на всех доступных интерфейсах
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        // номер порта, на котором сервер будет слушать входящие соединения
        serverAddr.sin_port = htons(PORT);

        if (bind(descriptor_server_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            Logger::error_log("Socket: Ошибка связывания сокета.");
            close(descriptor_server_socket);
            descriptor_server_socket = -1;
            std::this_thread::sleep_for(std::chrono::seconds(40)); // Ждем 40 секунд
            --count;
            continue;
        }

        if (listen(descriptor_server_socket, BACKLOG) < 0) {
            Logger::error_log("Socket: Ошибка при прослушивании порта.");
            close(descriptor_server_socket);
            descriptor_server_socket = -1;
            std::this_thread::sleep_for(std::chrono::seconds(40)); // Ждем 40 секунд
            --count;
            continue;
        }

        // Переопределяем строку через ostringstream
        std::ostringstream os;
        os << "Socket: Сервер запущен на порту " << PORT;
        Logger::info_log(os.str());
        count = 0;
    }
}

ServerSocket::~ServerSocket() {
    if (IsOpen()) {
        close(descriptor_server_socket);
        delete instance;
        Logger::info_log("Socket: Close Server Socket");
    }
}

// Метод для получения единственного экземпляра класса
ServerSocket* ServerSocket::getInstance() {
    if (instance == nullptr) {
        instance = new ServerSocket();
    }
    return instance;
}
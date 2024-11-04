#include <arpa/inet.h>
#include <unistd.h>

#include "server_socket.hpp"
#include "logger.hpp"

// создание серверного сокета
ServerSocket::ServerSocket() {

    descriptor_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptor_server_socket < 0) {
        Logger::error_log("Socket: Не удалось создать сокет.");
        return;
    }

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
        return;
    }

    if (listen(descriptor_server_socket, BACKLOG) < 0) {
        Logger::error_log("Socket: Ошибка при прослушивании порта.");
        close(descriptor_server_socket);
        descriptor_server_socket = -1;
        return;
    }

    Logger::info_log("Socket: Сервер запущен на порту " + PORT);
}

ServerSocket::~ServerSocket() {
    if (IsOpen()) {
        close(descriptor_server_socket);
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
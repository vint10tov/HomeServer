#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

#include "server_socket.hpp"
#include "logger.hpp"

// создание серверного сокета
ServerSocket::ServerSocket() {

    descriptor_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (descriptor_server_socket < 0) {
        Logger::error_log("Socket: Не удалось создать сокет.");
        return;
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
        return;
    }

    if (listen(descriptor_server_socket, BACKLOG) < 0) {
        Logger::error_log("Socket: Ошибка при прослушивании порта.");
        close(descriptor_server_socket);
        descriptor_server_socket = -1;
        return;
    }

    // Переопределяем строку через ostringstream
    std::ostringstream os;
    os << "Socket: Сервер запущен на порту " << PORT;
    Logger::info_log(os.str());
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
#include <iostream>

#include "socket.hpp"

const int PORT = 8000;
const int BACKLOG = 5; // Максимальное количество ожидающих подключений

// создание серверного сокета
int creating_socket_server () {

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Не удалось создать сокет." << std::endl;
        return -1;
    }

    // хранения адресной информации для IPv4
    sockaddr_in serverAddr{};
    // протоколы IPv4
    serverAddr.sin_family = AF_INET; 
    // сервер будет принимать соединения на всех доступных интерфейсах
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // номер порта, на котором сервер будет слушать входящие соединения
    serverAddr.sin_port = htons(PORT);


    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка связывания сокета." << std::endl;
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, BACKLOG) < 0) {
        std::cerr << "Ошибка при прослушивании порта." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Сервер запущен на порту " << PORT << std::endl;
    return serverSocket;
}

// принятие подключения
int creating_socket_client (int serverSocket) {

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Ошибка при принятии подключения." << std::endl;
        return -1;
    }
    return clientSocket;
}
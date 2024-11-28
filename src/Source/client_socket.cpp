#include "client_socket.hpp"

// принятие подключения
ClientSocket::ClientSocket() {
    descriptor_server_socket = ServerSocket::getInstance()->GET_descriptor_server_socket();
    descriptor_client_socket = accept(descriptor_server_socket, nullptr, nullptr);
    if (descriptor_client_socket < 0) {
        Logger::warn_log("Socket: Ошибка при принятии подключения.");
    }
}

ClientSocket::~ClientSocket() {
    if (IsOpen()) {
        // Закрываем сокет
        close(descriptor_client_socket);
    }
}

// Чтение данных из сокета
bool ClientSocket::reading_data_socket(char * buffer) {
    bytes_read = recv(descriptor_client_socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Завершение строки
        return true;
    } else if (bytes_read == 0) {
        buffer[0] = '\0'; // Завершение строки
        Logger::warn_log("ClientSocket: Соединение закрыто удаленным хостом.");
        return false;
    } else {
        buffer[0] = '\0'; // Завершение строки
        Logger::warn_log("ClientSocket: Ошибка при получении данных.");
        return false;
    }
}

// Отправляем ответ клиенту
void ClientSocket::writing_data_socket(std::string response) {
    Logger::debug_log("ClientSocket: Отправляем ответ клиенту");
    // Logger::debug_log(response);
    send(descriptor_client_socket, response.c_str(), response.size(), 0);
}
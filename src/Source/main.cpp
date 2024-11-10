#include <thread>
#include <memory>

#include "server_socket.hpp"
#include "client_socket.hpp"
#include "handle_client.hpp"
#include "uart_uno.hpp"
#include "logger.hpp"

// Инициализация статических член-класса
UartUno* UartUno::instance = nullptr;
ServerSocket* ServerSocket::instance = nullptr;

int main() {

    // Получаем единственный экземпляр UartUno
    UartUno* uartuno = UartUno::getInstance();
    ServerSocket* server_socket =  ServerSocket::getInstance();

    if (server_socket->GET_descriptor_server_socket() > 0) {
        while (true) {
            std::shared_ptr<ClientSocket> client_socket = std::make_shared<ClientSocket>();
            //ClientSocket client_socket(server_socket);
            if (client_socket->IsOpen()) {
                // Создаем новый поток для обработки клиента
                std::thread clientThread([client_socket, uartuno](){handleClientBase handleClientBase(client_socket, uartuno);});
                // Отделяем поток, чтобы он работал независимо
                clientThread.detach();
            }
        }
    }

    return 0;
}
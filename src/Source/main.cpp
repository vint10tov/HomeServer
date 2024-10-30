#include <iostream>
#include <thread>

#include "socket.hpp"
#include "handle_client.hpp"

int main() {

    int serverSocket = creating_socket_server();
    if (serverSocket > 0) {
        while (true) {
            int clientSocket = creating_socket_client(serverSocket);
            if (clientSocket > 0) {
                // Создаем новый поток для обработки клиента
                std::thread clientThread(handleClient, clientSocket);
                // Отделяем поток, чтобы он работал независимо
                clientThread.detach();
            }
        }
    }

    close(serverSocket);
    std::cout << "Close Server Socket" << std::endl;

    return 0;
}

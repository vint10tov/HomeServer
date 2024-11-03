#include <thread>

#include "socket.hpp"
#include "handle_client.hpp"
#include "uart_uno.hpp"
#include "logger.hpp"

const char TTY[] = "/dev/ttyUSB0";

// Инициализация статических член-класса
UartUno* UartUno::instance = nullptr;
std::mutex UartUno::mutex_uno;

int main() {

    // Получаем единственный экземпляр UartUno
    UartUno* uartuno = UartUno::getInstance(TTY);

    int serverSocket = creating_socket_server();
    if (serverSocket > 0) {
        while (true) {
            int clientSocket = creating_socket_client(serverSocket);
            if (clientSocket > 0) {
                // Создаем новый поток для обработки клиента
                std::thread clientThread(handleClient, clientSocket, uartuno);
                // Отделяем поток, чтобы он работал независимо
                clientThread.detach();
            }
        }
    }

    close(serverSocket);
    Logger::info_log("Close Server Socket");

    return 0;
}

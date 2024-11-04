// главная функция
# pragma once

#include <memory>

#include "client_socket.hpp"
#include "uart_uno.hpp"

void handleClient(std::shared_ptr<ClientSocket> client_socket, UartUno* uartuno);
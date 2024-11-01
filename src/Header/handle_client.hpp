// главная функция
# pragma once

#include "uart_uno.hpp"

// размер буфера сокета
const int BUFFER_SIZE = 4096;

void handleClient(int clientSocket, UartUno* uartuno);
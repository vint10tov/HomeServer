#pragma once

#include <arpa/inet.h>
#include <unistd.h>

#include "server_socket.hpp"
#include "logger.hpp"

// размер буфера сокета
const int BUFFER_SIZE = 4096;

class ClientSocket {
    private:
        int descriptor_client_socket = -1;
        int descriptor_server_socket;
        int bytes_read;
    public:
        ClientSocket(ServerSocket* server_socket);
        ~ClientSocket();
        bool IsOpen() {return descriptor_client_socket > 0 ? true : false;}
        // Чтение данных из сокета
        bool reading_data_socket(char * buffer);
        // Отправляем ответ клиенту
        void writing_data_socket(std::string & response);
        int GET_descriptor_client_socket() {return descriptor_client_socket;}
};
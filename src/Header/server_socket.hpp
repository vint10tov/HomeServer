# pragma once

const int PORT = 8000;
const int BACKLOG = 5; // Максимальное количество ожидающих подключений

class ServerSocket {
    private:
        static ServerSocket* instance; // Указатель на единственный экземпляр
        int descriptor_server_socket = -1;
        // Закрытый конструктор для предотвращения создания экземпляров
        ServerSocket();
        // Закрытый деструктор
        ~ServerSocket();

    public:
        // Метод для получения единственного экземпляра класса
        static ServerSocket* getInstance();
        bool IsOpen() {return descriptor_server_socket > 0 ? true : false;}
        int GET_descriptor_server_socket() {return descriptor_server_socket;}
};
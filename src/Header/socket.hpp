# pragma once

#include <arpa/inet.h>
#include <unistd.h>

// создание серверного сокета
int creating_socket_server ();
// принятие подключения
int creating_socket_client (int serverSocket);
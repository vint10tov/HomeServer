#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <string.h>
#include <mutex>

#include "handle_client.hpp"
#include "request.hpp"
#include "response.hpp"
#include "parser_body_relay.hpp"
#include "uart_uno.hpp"

void handleClient(int clientSocket, UartUno* uartuno) {

    std::mutex coutMutex; // Мьютекс для синхронизации потоков
    char buffer[BUFFER_SIZE] = {0};
    std::string request_body; // тело запроса
    std::string response_body; // тело ответа
    std::string request_headers; // заголовки запроса
    std::string response_headers; // заголовки ответа

    // Чтение данных из сокета
    int bytes_read = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Завершение строки
    } else if (bytes_read == 0) {
        buffer[0] = '\0'; // Завершение строки
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Соединение закрыто удаленным хостом." << std::endl;
        return;
    } else {
        buffer[0] = '\0'; // Завершение строки
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cerr << "Ошибка при получении данных." << std::endl;
        close(clientSocket);
        return;
    }

    Request req_hed(buffer, bytes_read);

    switch (req_hed.GET_method()) {
        case Request::Method::GET: {
            Response respons_class(req_hed);
            response_headers = respons_class.GET_result_headers();
            response_body = respons_class.GET_response_body();
        }
            break;
        case Request::Method::POST: {
            Response respons_class(req_hed);
            int body = req_hed.GET_begin_request_body();
            request_body.assign(buffer + body, bytes_read);
            //std::lock_guard<std::mutex> lock(coutMutex); // для отладки
            //std::cout << request_body << std::endl; // для отладки
            if (req_hed.GET_url() == "/relay") {
                ParserBodyRelay pbr(request_body);
                for (size_t i = 0; i < pbr.GET_resul().size(); ++i) {
                    uartuno->sending_string(pbr.GET_resul().at(i));
                }
            }
            response_headers = respons_class.GET_result_headers();
            response_body = respons_class.GET_response_body();
        }
            break;
        case Request::Method::HEAD:
            break;
        case Request::Method::PUT:
            break;
        case Request::Method::DELETE:
            break;
        case Request::Method::OPTIONS:
            break;
        case Request::Method::PATCH:
            break;
        default:
            break;
    }


    // Открываем файл для записи
    //std::ofstream outputFile("output.txt", std::ios::app);
    //if (!outputFile) {
    //    std::cerr << "Не удалось открыть файл для записи." << std::endl;
    //}

    // Записываем строку в файл
    // outputFile << buffer;

    // Закрываем файл
    // outputFile.close();

    /* Чтение данных из сокета
    while (true) {
        int bytes_read = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read < 0) {
            std::cerr << "Ошибка при приеме данных." << std::endl;
            close(clientSocket);
            break;
        }
        if (bytes_read == 0) {
            break; // Завершение чтения данных
        }

        buffer[bytes_read] = '\0'; // Завершение строки
        request.append(buffer);
        
        // Проверка на окончание заголовков
        if (request.find("\r\n\r\n") != std::string::npos) {
            break; // Заголовки завершены
        }
    }*/

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Получено сообщение от клиента:\n" << std::endl;
        req_hed.Show_RequestHeders();
    }

    // Отправляем ответ клиенту
    std::string response = response_headers + response_body;
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Отправляем ответ клиенту:\n" << std::endl;
        std::cout << response_headers << std::endl;
    }
    send(clientSocket, response.c_str(), response.size(), 0);

    // Закрываем сокет
    close(clientSocket);
}
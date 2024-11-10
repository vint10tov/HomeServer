# pragma once

#include <memory>

#include "client_socket.hpp"
#include "request.hpp"
#include "response.hpp"

const char PATH[] = "../src"; // каталог с файлами фронтенда
const char RELAY_HTML[] = "/html/relay.html";
const char Content_Type[] = "Content-Type: ";
    const char text_html[] = "text/html";
    const char text_css[] = "text/css";
    const char text_js[] = "text/js";
    const char image_icon[] = "image/x-icon";
    const char image_jpeg[] = "image/jpeg";

class handleClientBase {
    public:
        handleClientBase(std::shared_ptr<ClientSocket> client_socket, UartUno * uartuno);
        ~handleClientBase();
    private:
        char buffer[BUFFER_SIZE] = {0};
        std::shared_ptr<ClientSocket> client_socket = nullptr;
        UartUno * uartuno = nullptr;
        Request * request = nullptr;
        Response * response = nullptr;
        // отправка ответа клиенту
        void sending_response_client();
        void relay();
};
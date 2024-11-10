#include "handle_client.hpp"
#include "template_relay.hpp"
#include "parser_body_relay.hpp"
#include "uart_uno.hpp"

handleClientBase::handleClientBase(std::shared_ptr<ClientSocket> client_socket, UartUno * uartuno) {
    this->uartuno = uartuno;
    this->client_socket = client_socket;
    // Чтение данных из сокета
    if (client_socket->IsOpen()) {
        client_socket->reading_data_socket(buffer);
        // запрос
        request = new Request(buffer, BUFFER_SIZE);
        // ответ
        response = new Response();
        // получено сообщение от клиента
        Logger::debug_log("handleClientBase: Получено сообщение от клиента");
        request->Show_RequestHeders();
        if (request->GET_url() == "/relay") {
            relay();
        } else {
            response->SET_status_code_500();
        }
        sending_response_client();
    }
}

handleClientBase::~handleClientBase() {
    if (request != nullptr)
        delete request;
    if (response != nullptr)
        delete response;
}

// отправка ответа клиенту
void handleClientBase::sending_response_client() {
    client_socket->writing_data_socket(response->GET_result());
}

void handleClientBase::relay() {
    if (request->GET_method()  == Request::Method::GET) {
        response->SET_status_code_200();
        response->SET_headlines(Content_Type, text_html);
        response->Upload_text_file(std::string(PATH) + std::string(RELAY_HTML), uartuno, TemplateHTML::replace_matches);
    } else if (request->GET_method()  == Request::Method::POST) {
        std::string body = request->reading_request_body();
        //Logger::debug_log("handleClientBase: " + body);
        ParserBodyRelay pbr(body);
        for (size_t i = 0; i < pbr.GET_resul().size(); ++i) {
            UartUno::getInstance()->sending_string(pbr.GET_resul().at(i));
        }
        response->SET_status_code_200();
        response->SET_headlines(Content_Type, text_html);
        response->Upload_text_file(std::string(PATH) + std::string(RELAY_HTML), uartuno, TemplateHTML::replace_matches);
    } else {
        //
    }
}
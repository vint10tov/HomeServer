#include "handle_client.hpp"
#include "template_relay.hpp"
#include "parser_body_relay.hpp"
#include "uart_uno.hpp"
#include "smart_home.hpp"

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
    static const int SIZE_BUF = 20;
    uint8_t bufer_in[SIZE_BUF];
    uint8_t bufer_out[SIZE_BUF];
    SmartHome smart_home(0);
    
    if (request->GET_method() == Request::Method::GET) {
        RequestFromServer rfs;
        rfs.SET_PING();
        Logger::info_log(rfs.show_request());
        rfs.serialize(bufer_in, SIZE_BUF);
        
        if (!uartuno->sending_string(bufer_in, bufer_out, SIZE_BUF)) {
            response->SET_status_code_500();
            return;
        }
        
        smart_home.deserialize(bufer_out, SIZE_BUF);
        response->SET_status_code_200();
        response->SET_headlines(Content_Type, text_html);
        response->Upload_text_file(std::string(PATH) + std::string(RELAY_HTML), smart_home, TemplateHTML::replace_matches);
        
    } else if (request->GET_method() == Request::Method::POST) {
        std::string body = request->reading_request_body();
        ParserBodyRelay pbr(body);
        
        pbr.GET_request_from_server().serialize(bufer_in, SIZE_BUF);
        Logger::info_log(pbr.GET_request_from_server().show_request());
        
        if (!uartuno->sending_string(bufer_in, bufer_out, SIZE_BUF)) {
            response->SET_status_code_500();
            return;
        }

        smart_home.deserialize(bufer_out, SIZE_BUF);
        response->SET_status_code_200();
        response->SET_headlines(Content_Type, text_html);
        response->Upload_text_file(std::string(PATH) + std::string(RELAY_HTML), smart_home, TemplateHTML::replace_matches);
        
    } else {
        response->SET_status_code_404();
    }
}
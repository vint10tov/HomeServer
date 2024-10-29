#include "parser_uno.hpp"

Parser::Parser(char * buf, uint8_t sizebuf) {
    while (i < sizebuf && buf[i] != '/' && buf[i] != '=' && error) {
        iterat_buf(buf, sizebuf);
        TypeDefinition();
    }
    if (buf[i] == '/' && error) {
        return;
    } else if (buf[i] == '=' && error) {
        ++i;
        while (i < sizebuf && buf[i] != '/' && error) {
            iterat_buf(buf, sizebuf);
            ValueUint();
        }
    }
    if (buf[i] != '/')
        error = false;
}

void Parser::iterat_buf(char * b, uint8_t s) {
    uint8_t z = 0;
    while (i < s && b[i] != '/' && z < SIZE_TEMP_BUF - 1) {
        temp[z] = b[i];
        ++i;
                ++z;
    }
    temp[z] = '\0';
    if (b[i] != '/' || z == 0)
        error = false;
    ++i;
}

void Parser::ValueUint() {
    if (value_counter < SIZE_VALUE) {
        value[value_counter] = atoi(temp);
        ++value_counter;
    } else {
        error = false;
    }
}

void Parser::TypeDefinition () {
    if (strcmp("ok", temp) == 0) {
        type = Parser::Type::OK;
    } else if (strcmp("er", temp) == 0) {
        type = Parser::Type::ERROR;
    } else 
        error = false;
}


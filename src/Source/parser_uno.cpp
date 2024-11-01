#include "parser_uno.hpp"

ParserUno::ParserUno(std::string & bufer) {
    uint sizebuf = bufer.size();
    while (i < sizebuf && bufer[i] != '/' && bufer[i] != '=' && error) {
        iterat_buf(bufer, sizebuf);
        TypeDefinition();
    }
    if (bufer[i] == '/' && error) {
        return;
    } else if (bufer[i] == '=' && error) {
        ++i;
        while (i < sizebuf && bufer[i] != '/' && error) {
            iterat_buf(bufer, sizebuf);
            ValueUint();
        }
    }
    if (bufer[i] != '/')
        error = false;
}

void ParserUno::iterat_buf(std::string & b, uint s) {
    uint z = 0;
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

void ParserUno::ValueUint() {
    if (value_counter < SIZE_VALUE) {
        value[value_counter] = atoi(temp);
        ++value_counter;
    } else {
        error = false;
    }
}

void ParserUno::TypeDefinition () {
    if (strcmp("ok", temp) == 0) {
        type = ParserUno::Type::OK;
    } else if (strcmp("er", temp) == 0) {
        type = ParserUno::Type::ERROR;
    } else 
        error = false;
}
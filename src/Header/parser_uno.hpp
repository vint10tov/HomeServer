#pragma once

#include <string.h>
#include <stdlib.h>
#include <cstdint>

const uint8_t SIZE_TEMP_BUF = 8;
const uint8_t SIZE_VALUE = 7;

class Parser {
    public:

        enum class Type {OK, ERROR, DEFAULT};

    private:

        bool error = true; // ошибки при парсинге - false    
        char temp[SIZE_TEMP_BUF]; // буфер для разбиения поступившего массива
        uint8_t i = 0; // позиция в массиве
        uint8_t value_counter = 0; // количество значений VALUE

        uint8_t value[SIZE_VALUE] {0};
        Parser::Type type = Parser::Type::DEFAULT;

        // разбиение массива на строки, возвращает false если есть ощибки
        void iterat_buf(char * b, uint8_t s);

        // 
        void ValueUint();
        void TypeDefinition ();

    public:
        
        Parser(char * buf, uint8_t sizebuf);

        bool GET_error() const {return error;}
        Parser::Type GET_type() const {return type;}
        uint8_t GET_value(uint8_t i) {return value[i];}
};

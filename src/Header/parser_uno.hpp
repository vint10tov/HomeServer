#pragma once

#include <string>
#include <string.h>

const uint SIZE_TEMP_BUF = 8;
const uint SIZE_VALUE = 7;

class ParserUno {
    public:

        enum class Type {OK, ERROR, DEFAULT};

    private:

        bool error = true; // ошибки при парсинге - false    
        char temp[SIZE_TEMP_BUF]; // буфер для разбиения поступившего массива
        uint i = 0; // позиция в массиве
        uint value_counter = 0; // количество значений VALUE

        uint value[SIZE_VALUE] {0};
        ParserUno::Type type = ParserUno::Type::DEFAULT;

        // разбиение массива на строки, возвращает false если есть ощибки
        void iterat_buf(std::string & b, uint s);

        // 
        void ValueUint();
        void TypeDefinition ();

    public:
        
        ParserUno(std::string & bufer);

        bool GET_error() const {return error;}
        ParserUno::Type GET_type() const {return type;}
        uint GET_value(uint i) {return value[i];}
};
#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>

class ParserBodyRelay {
public:
    ParserBodyRelay(std::string &req_body);
    const std::vector<std::string> & GET_resul() {return resul;}

private:
    std::vector<std::string> resul;
    uint count;
    std::vector<std::string> key;
    std::vector<int> value;

    void parse(const std::string &req_body);
    int parseValue(const std::string &v) const;
};
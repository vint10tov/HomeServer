#pragma once

#include <string>

#include "uart_uno.hpp"

const std::string get_time = "gt/tm//;";
const std::string get_relay = "gt/rl/pn//;";
const std::string get_mod2_0 = "gt/rl/md/=2/0//;";
const std::string get_mod2_1 = "gt/rl/md/=2/1//;";

std::string replace_matches(std::string& input, UartUno* uartuno);
bool is_number(const std::string& str);
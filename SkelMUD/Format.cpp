//
// Created by skelton on 3/19/16.
//

#include "Format.h"

const std::string Format::NONE = "";
const std::string Format::RESET = "\33[0m";
const std::string Format::BOLD = "\33[1m";
const std::string Format::NORMAL = "\33[22m";
const std::string Format::BLACK = "\33[30m";
const std::string Format::RED = "\33[31m";
const std::string Format::GREEN = "\33[32m";
const std::string Format::YELLOW = "\33[33m";
const std::string Format::BLUE = "\33[34m";
const std::string Format::MAGENTA = "\33[35m";
const std::string Format::CYAN = "\33[36m";
const std::string Format::WHITE = "\33[37m";

const std::string Format::UP = "\33[1A";

const std::string Format::FRONT_LINE = "\33[G";
const std::string Format::ERASE = "\33[2K";

const std::string Format::SAVE = "\33[s";
const std::string Format::RESTORE = "\33[u";
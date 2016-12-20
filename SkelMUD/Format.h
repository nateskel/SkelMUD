//
// Created by skelton on 3/19/16.
//

#ifndef SKELMUD_FORMAT_H
#define SKELMUD_FORMAT_H

#include <string>

class Format {
public:
    static const std::string NONE;
    static const std::string RESET;
    static const std::string BOLD;
    static const std::string NORMAL;
    static const std::string BLACK;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;

    static const std::string UP;
    static const std::string NL;
    static const std::string FRONT_LINE;
    static const std::string ERASE;

    static const std::string SAVE;
    static const std::string RESTORE;

};


#endif //SKELMUD_FORMAT_H

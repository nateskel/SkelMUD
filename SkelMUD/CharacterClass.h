//
// Created by skelton on 8/26/15.
//

#ifndef SKELMUD_CHARACTERCLASS_H
#define SKELMUD_CHARACTERCLASS_H


#include <string>

class CharacterClass {
public:
    CharacterClass(const std::string &m_name) : m_name(m_name) { }

    std::string GetName();

private:
    std::string m_name;

};


#endif //SKELMUD_CHARACTERCLASS_H

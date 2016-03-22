//
// Created by nate on 7/8/16.
//

#ifndef SKELMUD_CHARACTERCLASSES_H
#define SKELMUD_CHARACTERCLASSES_H


#include "CharacterClass.h"
#include "../Races/Race.h"

class CharacterClasses {

private:
    std::map<std::string, CharacterClass> m_class_map;
public:
    void LoadClasses(std::string filename);
    void AddClass(CharacterClass character_class);
    std::map<std::string, CharacterClass> GetClasses();
    std::map<int, CharacterClass> EnumerateClasses();
};


#endif //SKELMUD_CHARACTERCLASSES_H

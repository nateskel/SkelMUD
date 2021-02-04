//
// Created by nate on 7/8/16.
//

#ifndef SKELMUD_CHARACTERCLASS_H
#define SKELMUD_CHARACTERCLASS_H

#include <string>
#include <map>

class CharacterClass {
private:
    std::map<std::string, int> m_skill_map;
    std::string m_name;
public:
    CharacterClass() : m_name("") { }
    CharacterClass(std::string name) : m_name(name) { }
    void AddSkill(const std::string& skill_name, int skill_max_level);
    int GetSkill(const std::string& name);
    std::string GetName();
};


#endif //SKELMUD_CHARACTERCLASS_H

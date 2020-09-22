//
// Created by nate on 7/8/16.
//

#include "CharacterClass.h"


void CharacterClass::AddSkill(const std::string &skill_name, int skill_max_level) {
    m_skill_map[skill_name] = skill_max_level;
}

int CharacterClass::GetSkill(const std::string &name) {
    if(m_skill_map.find(name) == m_skill_map.end())
        return 0;
    return m_skill_map[name];
}


std::string CharacterClass::GetName() {
    return m_name;
}
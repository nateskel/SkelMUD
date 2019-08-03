//
// Created by nate on 7/8/16.
//

#ifndef SKELMUD_SKILL_H
#define SKELMUD_SKILL_H

#include <string>

class Skill {
private:
    std::string m_skill_name;
    int m_level;
    int m_max_level;
public:
    enum SKILL_MOD {
        AOE = 1,
        DOT = 2,
        SELF = 4,
        WEAKEN = 8,
        BUFF = 16,
        DAMAGE = 32,
        HEAL = 64
    };
};


#endif //SKELMUD_SKILL_H

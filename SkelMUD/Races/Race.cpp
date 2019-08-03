//
// Created by nate on 7/8/16.
//

#include "Race.h"


Race::Race(const std::string &name, const std::string &str, const std::string &end, const std::string &intel,
           const std::string &dex, const std::string &skill) {
    m_race_name = name;
    m_strength = atoi(str.c_str());
    m_endurance = atoi(end.c_str());
    m_intelligence = atoi(intel.c_str());
    m_dexterity = atoi(dex.c_str());
    m_skill = atoi(skill.c_str());
}
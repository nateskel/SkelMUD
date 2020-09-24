//
// Created by nate on 7/8/16.
//

#ifndef SKELMUD_RACE_H
#define SKELMUD_RACE_H

#include <string>
#include <map>

class Race {
private:
    int m_strength;
    int m_endurance;
    int m_intelligence;
    int m_dexterity;
    int m_skill;
    std::string m_race_name;
public:
    Race() : m_strength(1), m_endurance(1), m_intelligence(1), m_dexterity(1), m_skill(1) { }

    Race(std::string name, int str, int end, int intel, int dex, int skill) : m_race_name(name),
                                                                                        m_strength(str),
                                                                                        m_endurance(end),
                                                                                        m_intelligence(intel),
                                                                                        m_dexterity(dex),
                                                                                        m_skill(skill) { }

    Race(const std::string &name, const std::string &str, const std::string &end, const std::string &intel,
         const std::string &dex, const std::string &skill);

    int getStrength() const {
        return m_strength;
    }

    void setStrength(int m_strength) {
        Race::m_strength = m_strength;
    }

    int getEndurance() const {
        return m_endurance;
    }

    void setEndurance(int m_endurance) {
        Race::m_endurance = m_endurance;
    }

    int getIntelligence() const {
        return m_intelligence;
    }

    void setIntelligence(int m_intelligence) {
        Race::m_intelligence = m_intelligence;
    }

    int getDexterity() const {
        return m_dexterity;
    }

    void setDexterity(int m_dexterity) {
        Race::m_dexterity = m_dexterity;
    }

    int getSkill() const {
        return m_skill;
    }

    void setSkill(int m_skill) {
        Race::m_skill = m_skill;
    }


    const std::string &getRace_name() const {
        return m_race_name;
    }

    void setRace_name(const std::string &m_race_name) {
        Race::m_race_name = m_race_name;
    }
};


#endif //SKELMUD_RACE_H

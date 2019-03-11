//
// Created by drax on 2/28/19.
//

#ifndef SKELMUD_MODIFIER_H
#define SKELMUD_MODIFIER_H


class Modifier {
public:
private:
    int m_max_hp;
    int m_max_sp;
    int m_max_stam;
public:
    int GetM_max_hp() const;

    void SetM_max_hp(int m_max_hp);

    int GetM_max_sp() const;

    void SetM_max_sp(int m_max_sp);

    int GetM_max_stam() const;

    void SetM_max_stam(int m_max_stam);

    int GetM_strength() const;

    void SetM_strength(int m_strength);

    int GetM_endurance() const;

    void SetM_endurance(int m_endurance);

    int GetM_intelligence() const;

    void SetM_intelligence(int m_intelligence);

    int GetM_dexterity() const;

    void SetM_dexterity(int m_dexterity);

    int GetM_skill() const;

    void SetM_skill(int m_skill);

private:
    int m_strength;
    int m_endurance;
    int m_intelligence;
    int m_dexterity;
    int m_skill;

};


#endif //SKELMUD_MODIFIER_H

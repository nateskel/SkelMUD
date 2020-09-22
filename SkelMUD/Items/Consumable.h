//
// Created by drax on 2/28/19.
//

#ifndef SKELMUD_CONSUMABLE_H
#define SKELMUD_CONSUMABLE_H

#include <string>
#include "ItemMixin.h"
#include "../Mixin.h"

class Consumable : public Mixin {
public:
    Consumable();

    virtual ~Consumable();

    int GetHP() const;

    void SetHP(int m_hp);

    int GetMaxHP() const;

    void SetMaxHP(int m_max_hp);

    int GetSP() const;

    void SetSP(int m_sp);

    int GetMaxSP() const;

    void SetMaxSP(int m_max_sp);

    int GetStamina() const;

    void SetStamina(int m_stam);

    int GetMaxStamina() const;

    void SetMaxStamina(int m_max_stam);

    int GetStrength() const;

    void SetStrength(int m_str);

    int GetEndurance() const;

    void SetEndurance(int m_end);

    int GetIntelligence() const;

    void SetIntelligence(int m_int);

    int GetDexterity() const;

    void SetDexterity(int m_dex);

    int GetSkill() const;

    void SetSkill(int m_skill);

    int GetTimer() const;

    void SetTimer(int m_timer);

    const std::string &GetEat() const;

    void SetEat(const std::string &m_eat);

    const std::string &GetDrink() const;

    void SetDrink(const std::string &m_drink);

private:
    int m_hp;
    int m_max_hp;
    int m_sp;
    int m_max_sp;
    int m_stam;
    int m_max_stam;
    int m_str;
    int m_end;
    int m_int;
    int m_dex;
    int m_skill;
    int m_timer;
    std::string m_eat;
    std::string m_drink;
};


#endif //SKELMUD_CONSUMABLE_H

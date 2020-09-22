//
// Created by drax on 2/28/19.
//

#ifndef SKELMUD_WIELDABLE_H
#define SKELMUD_WIELDABLE_H


#include <string>
#include "ItemMixin.h"
#include "../Mixin.h"

enum WeaponClass {RANGED, MELEE};
enum HandType {MAIN_HAND, OFF_HAND, EITHER_HAND, TWO_HAND};
enum ScaleAttribute {STR, DEX, INT, END, SKILL};

class Wieldable : public Mixin {
public:
    Wieldable();

    WeaponClass GetWeaponClass() const;

    void SetWeaponClass(WeaponClass m_weapon_class);

    const std::string &GetWeaponType() const;

    void SetWeaponType(const std::string &m_weapon_type);

    HandType GetHandType() const;

    void SetHandType(HandType m_hand_type);

    int GetMinDamage() const;

    void SetMinDamage(int m_min_damage);

    int GetMaxDamage() const;

    void SetMaxDamage(int m_max_damage);

    int GetLevel() const;

    void SetLevel(int m_level);

    ScaleAttribute GetScale() const;

    void SetScale(ScaleAttribute m_scale);

    int GetAccuracy() const;

    void SetAccuracy(int m_accuracy);

    const std::string &GetNormalHit() const;

    void SetNormalHit(const std::string &m_normal_hit);

    const std::string &GetCriticalHit() const;

    void SetCriticalHit(const std::string &m_critical_hit);

    const std::string &GetNormalHitOther() const;

    void SetNormalHitOther(const std::string &normal_hit_other);

    const std::string &GetCriticalHitOther() const;

    void SetCriticalHitOther(const std::string &critical_hit_other);

private:
    WeaponClass m_weapon_class;
    std::string m_weapon_type;
    HandType m_hand_type;
    int m_min_damage;
    int m_max_damage;
    int m_level;
    ScaleAttribute m_scale;
    int m_accuracy;
    std::string m_normal_hit;
    std::string m_critical_hit;
    std::string m_normal_hit_other;
    std::string m_critical_hit_other;
};


#endif //SKELMUD_WIELDABLE_H

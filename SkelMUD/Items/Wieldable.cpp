//
// Created by drax on 2/28/19.
//

#include "Wieldable.h"

Wieldable::Wieldable() {

}

WeaponClass Wieldable::GetWeaponClass() const {
    return m_weapon_class;
}

void Wieldable::SetWeaponClass(WeaponClass m_weapon_class) {
    Wieldable::m_weapon_class = m_weapon_class;
}

const std::string &Wieldable::GetWeaponType() const {
    return m_weapon_type;
}

void Wieldable::SetWeaponType(const std::string &m_weapon_type) {
    Wieldable::m_weapon_type = m_weapon_type;
}

HandType Wieldable::GetHandType() const {
    return m_hand_type;
}

void Wieldable::SetHandType(HandType m_hand_type) {
    Wieldable::m_hand_type = m_hand_type;
}

int Wieldable::GetMinDamage() const {
    return m_min_damage;
}

void Wieldable::SetMinDamage(int m_min_damage) {
    Wieldable::m_min_damage = m_min_damage;
}

int Wieldable::GetMaxDamage() const {
    return m_max_damage;
}

void Wieldable::SetMaxDamage(int m_max_damage) {
    Wieldable::m_max_damage = m_max_damage;
}

int Wieldable::GetLevel() const {
    return m_level;
}

void Wieldable::SetLevel(int m_level) {
    Wieldable::m_level = m_level;
}

ScaleAttribute Wieldable::GetScale() const {
    return m_scale;
}

void Wieldable::SetScale(ScaleAttribute m_scale) {
    Wieldable::m_scale = m_scale;
}

int Wieldable::GetAccuracy() const {
    return m_accuracy;
}

void Wieldable::SetAccuracy(int m_accuracy) {
    Wieldable::m_accuracy = m_accuracy;
}

const std::string &Wieldable::GetNormalHit() const {
    return m_normal_hit;
}

void Wieldable::SetNormalHit(const std::string &m_normal_hit) {
    Wieldable::m_normal_hit = m_normal_hit;
}

const std::string &Wieldable::GetCriticalHit() const {
    return m_critical_hit;
}

void Wieldable::SetCriticalHit(const std::string &m_critical_hit) {
    Wieldable::m_critical_hit = m_critical_hit;
}

const std::string &Wieldable::GetNormalHitOther() const {
    return m_normal_hit_other;
}

void Wieldable::SetNormalHitOther(const std::string &normal_hit_other) {
    m_normal_hit_other = normal_hit_other;
}

const std::string &Wieldable::GetCriticalHitOther() const {
    return m_critical_hit_other;
}

void Wieldable::SetCriticalHitOther(const std::string &critical_hit_other) {
    m_critical_hit_other = critical_hit_other;
}

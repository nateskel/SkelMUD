//
// Created by drax on 2/28/19.
//

#include "Modifier.h"

int Modifier::GetM_max_hp() const {
    return m_max_hp;
}

void Modifier::SetM_max_hp(int m_max_hp) {
    Modifier::m_max_hp = m_max_hp;
}

int Modifier::GetM_max_sp() const {
    return m_max_sp;
}

void Modifier::SetM_max_sp(int m_max_sp) {
    Modifier::m_max_sp = m_max_sp;
}

int Modifier::GetM_max_stam() const {
    return m_max_stam;
}

void Modifier::SetM_max_stam(int m_max_stam) {
    Modifier::m_max_stam = m_max_stam;
}

int Modifier::GetM_strength() const {
    return m_strength;
}

void Modifier::SetM_strength(int m_strength) {
    Modifier::m_strength = m_strength;
}

int Modifier::GetM_endurance() const {
    return m_endurance;
}

void Modifier::SetM_endurance(int m_endurance) {
    Modifier::m_endurance = m_endurance;
}

int Modifier::GetM_intelligence() const {
    return m_intelligence;
}

void Modifier::SetM_intelligence(int m_intelligence) {
    Modifier::m_intelligence = m_intelligence;
}

int Modifier::GetM_dexterity() const {
    return m_dexterity;
}

void Modifier::SetM_dexterity(int m_dexterity) {
    Modifier::m_dexterity = m_dexterity;
}

int Modifier::GetM_skill() const {
    return m_skill;
}

void Modifier::SetM_skill(int m_skill) {
    Modifier::m_skill = m_skill;
}

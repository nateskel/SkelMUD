//
// Created by drax on 2/28/19.
//

#include "Consumable.h"

Consumable::Consumable() {
    m_hp = 0;
    m_max_hp = 0;
    m_sp = 0;
    m_max_sp = 0;
    m_stam = 0;
    m_max_stam = 0;
    m_str = 0;
    m_end = 0;
    m_dex = 0;
    m_int = 0;
    m_skill = 0;
    m_timer = 0;
    m_eat = "";
    m_drink = "";
}

int Consumable::GetHP() const {
    return m_hp;
}

void Consumable::SetHP(int m_hp) {
    Consumable::m_hp = m_hp;
}

int Consumable::GetMaxHP() const {
    return m_max_hp;
}

void Consumable::SetMaxHP(int m_max_hp) {
    Consumable::m_max_hp = m_max_hp;
}

int Consumable::GetSP() const {
    return m_sp;
}

void Consumable::SetSP(int m_sp) {
    Consumable::m_sp = m_sp;
}

int Consumable::GetMaxSP() const {
    return m_max_sp;
}

void Consumable::SetMaxSP(int m_max_sp) {
    Consumable::m_max_sp = m_max_sp;
}

int Consumable::GetStamina() const {
    return m_stam;
}

void Consumable::SetStamina(int m_stam) {
    Consumable::m_stam = m_stam;
}

int Consumable::GetMaxStamina() const {
    return m_max_stam;
}

void Consumable::SetMaxStamina(int m_max_stam) {
    Consumable::m_max_stam = m_max_stam;
}

int Consumable::GetStrength() const {
    return m_str;
}

void Consumable::SetStrength(int m_str) {
    Consumable::m_str = m_str;
}

int Consumable::GetEndurance() const {
    return m_end;
}

void Consumable::SetEndurance(int m_end) {
    Consumable::m_end = m_end;
}

int Consumable::GetIntelligence() const {
    return m_int;
}

void Consumable::SetIntelligence(int m_int) {
    Consumable::m_int = m_int;
}

int Consumable::GetDexterity() const {
    return m_dex;
}

void Consumable::SetDexterity(int m_dex) {
    Consumable::m_dex = m_dex;
}

int Consumable::GetSkill() const {
    return m_skill;
}

void Consumable::SetSkill(int m_skill) {
    Consumable::m_skill = m_skill;
}

int Consumable::GetTimer() const {
    return m_timer;
}

void Consumable::SetTimer(int m_timer) {
    Consumable::m_timer = m_timer;
}

const std::string &Consumable::GetEat() const {
    return m_eat;
}

void Consumable::SetEat(const std::string &m_eat) {
    Consumable::m_eat = m_eat;
}

const std::string &Consumable::GetDrink() const {
    return m_drink;
}

void Consumable::SetDrink(const std::string &m_drink) {
    Consumable::m_drink = m_drink;
}

Consumable::~Consumable() {

}

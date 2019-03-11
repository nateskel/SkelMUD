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
}

int Consumable::GetM_hp() const {
    return m_hp;
}

void Consumable::SetM_hp(int m_hp) {
    Consumable::m_hp = m_hp;
}

int Consumable::GetM_max_hp() const {
    return m_max_hp;
}

void Consumable::SetM_max_hp(int m_max_hp) {
    Consumable::m_max_hp = m_max_hp;
}

int Consumable::GetM_sp() const {
    return m_sp;
}

void Consumable::SetM_sp(int m_sp) {
    Consumable::m_sp = m_sp;
}

int Consumable::GetM_max_sp() const {
    return m_max_sp;
}

void Consumable::SetM_max_sp(int m_max_sp) {
    Consumable::m_max_sp = m_max_sp;
}

int Consumable::GetM_stam() const {
    return m_stam;
}

void Consumable::SetM_stam(int m_stam) {
    Consumable::m_stam = m_stam;
}

int Consumable::GetM_max_stam() const {
    return m_max_stam;
}

void Consumable::SetM_max_stam(int m_max_stam) {
    Consumable::m_max_stam = m_max_stam;
}

int Consumable::GetM_str() const {
    return m_str;
}

void Consumable::SetM_str(int m_str) {
    Consumable::m_str = m_str;
}

int Consumable::GetM_end() const {
    return m_end;
}

void Consumable::SetM_end(int m_end) {
    Consumable::m_end = m_end;
}

int Consumable::GetM_int() const {
    return m_int;
}

void Consumable::SetM_int(int m_int) {
    Consumable::m_int = m_int;
}

int Consumable::GetM_dex() const {
    return m_dex;
}

void Consumable::SetM_dex(int m_dex) {
    Consumable::m_dex = m_dex;
}

int Consumable::GetM_skill() const {
    return m_skill;
}

void Consumable::SetM_skill(int m_skill) {
    Consumable::m_skill = m_skill;
}

int Consumable::GetM_timer() const {
    return m_timer;
}

void Consumable::SetM_timer(int m_timer) {
    Consumable::m_timer = m_timer;
}

#include "Entity.h"
#include "Logger.h"

Entity::Entity(int max_hp, int max_sp, int max_stamina) {
    m_max_hp = m_hp = max_hp;
    m_max_sp = m_sp = max_sp;
    m_max_stamina = m_stamina = max_stamina;
    m_in_ship = false;
}

Entity::Entity() {
    m_max_hp = m_hp = 100;
    m_max_sp = m_sp = 100;
    m_max_stamina = m_stamina = 100;
    m_in_ship = false;
}

Entity::~Entity() {

}

void Entity::SetPlanetID(int planet_id) {
    m_planet_id = planet_id;
}

int Entity::GetPlanetID() {
    return m_planet_id;
}

void Entity::SetShipID(int planet_id) {
    Logger::Info("Ship ID" + std::to_string(planet_id));
    m_planet_id = planet_id;
}

int Entity::GetShipID() {
    return m_planet_id;
}

int Entity::GetLocationID() {
    if(IsInShip()) {
        return m_ship_id;
    }
    else {
        return m_planet_id;
    }
}

bool Entity::IsInShip() {
    return m_in_ship;
}

void Entity::SetInShip(bool in_ship) {
    m_in_ship = in_ship;
}

void Entity::SetRoomID(int room_id) {
    m_room_id = room_id;
}

int Entity::GetRoomID() {
    return m_room_id;
}

void Entity::SetName(std::string name) {
    m_name = name;
}

std::string Entity::GetName() {
    return m_name;
}

void Entity::SetHP(int hp) {
    m_hp = hp;
}

void Entity::SetSP(int sp) {
    m_sp = sp;
}

void Entity::SetStamina(int stamina) {
    m_stamina = stamina;
}

void Entity::SetMaxHP(int maxhp) {
    m_max_hp = maxhp;
}

void Entity::SetMaxSP(int maxsp) {
    m_max_sp = maxsp;
}

void Entity::SetMaxStamina(int maxstamina) {
    m_max_stamina = maxstamina;
}

int Entity::GetHP() {
    return m_hp;
}

int Entity::GetSP() {
    return m_sp;
}

int Entity::GetStamina() {
    return m_stamina;
}

int Entity::GetMaxHP() {
    return m_max_hp;
}

int Entity::GetMaxSP() {
    return m_max_sp;
}

int Entity::GetMaxStamina() {
    return m_max_stamina;
}
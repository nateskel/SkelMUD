#include "Entity.h"
#include "Logger.h"
#include "Planets/Planet.h"
#include "Areas/Room.h"
#include "Areas/Area.h"
#include "Ships/Ship.h"
//#include "Items/Item.h"

Entity::Entity(int max_hp, int max_sp, int max_stamina) {
    m_max_hp = m_hp = max_hp;
    m_max_sp = m_sp = max_sp;
    m_max_stamina = m_stamina = max_stamina;
    m_in_ship = false;
    m_target = nullptr;
}

Entity::Entity() {
    m_max_hp = m_hp = 100;
    m_max_sp = m_sp = 100;
    m_max_stamina = m_stamina = 100;
    m_in_ship = false;
    m_target = nullptr;
}

Entity::~Entity() {

}

//void Entity::SetPlanetID(int planet_id) {
//    m_planet_id = planet_id;
//}
//
//int Entity::GetPlanetID() {
//    return m_planet_id;
//}

void Entity::SetPlanet(std::shared_ptr<Planet> planet) {
    m_planet = planet;
}

std::shared_ptr<Planet> Entity::GetPlanet() {
    return m_planet.lock();
}

void Entity::SetShipID(int ship_id) {
    m_ship_id = ship_id;
}

int Entity::GetShipID() {
    return m_ship_id;
}

int Entity::GetLocationID() {
    if(IsInShip())
        return m_ship_id;
    else
        return m_planet_id;
}

void Entity::SetLocationID(int id) {
    if(IsInShip())
        m_ship_id = id;
    else
        m_planet_id = id;
}

std::shared_ptr<Area> Entity::GetLocation() {
    if(IsInShip())
        return m_ship.lock();
    else
        return m_planet.lock();
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

void Entity::SetRoom(std::shared_ptr<Room> room) {
    m_room = room;
}

std::shared_ptr<Room> Entity::GetRoom() {
    return m_room.lock();
}

void Entity::SetShip(std::shared_ptr<Ship> ship) {
    m_ship = ship;
}

std::shared_ptr<Ship> Entity::GetShip() {
    return m_ship.lock();
}

void Entity::BeginFighting(std::shared_ptr<Entity> target) {
    m_target = target;
}

void Entity::AddAttacker(std::shared_ptr<Entity> attacker) {
    Logger::Info(GetName() + ": add attacker " + attacker->GetName());
    m_attackers.push_back(attacker);
}

std::vector<std::shared_ptr<Entity>> Entity::GetAttackers() {
    return m_attackers;
}

void Entity::RemoveAttacker(std::shared_ptr<Entity> attacker) {
    auto found = std::find(m_attackers.begin(), m_attackers.end(), attacker);
    if(found != m_attackers.end())
        m_attackers.erase(found);
}

void Entity::RemoveAllAttackers() {
    m_attackers.clear();
}

void Entity::StopFighting() {
    m_target = nullptr;
}

bool Entity::IsFighting() {
    return m_target != nullptr;
}

bool Entity::IsAttacked() {
    return !m_attackers.empty();
}

std::shared_ptr<Entity> Entity::GetTarget() {
    return m_target;
}

bool Entity::Damage(int amount) {
    m_hp -= amount;
    return m_hp <= 0;
}

void Entity::Heal(int amount) {
    Damage(-amount);
}

void Entity::Send(std::string data) {
    // Possible future use, for now this is overridden by Player
}

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <map>
#include <vector>

class Planet;
class Ship;
class Room;
class Area;
//class Item;

class Entity {
public:
    Entity(int max_hp, int max_sp, int max_stamina);

    Entity();

    ~Entity();

    void SetName(std::string name);

    std::string GetName();

    // void SetPlanetID(int planet_id);

    // int GetPlanetID();

    void SetShipID(int ship_id);

    int GetShipID();

    void SetRoomID(int room_id);

    int GetRoomID();

    int GetLocationID();

    void SetLocationID(int id);

    std::shared_ptr<Area> GetLocation();

    bool IsInShip();

    void SetInShip(bool in_ship);

    void SetPlanet(std::shared_ptr<Planet> planet);

    std::shared_ptr<Planet> GetPlanet();

    void SetShip(std::shared_ptr<Ship> ship);

    std::shared_ptr<Ship> GetShip();

    void SetRoom(std::shared_ptr<Room> room);

    std::shared_ptr<Room> GetRoom();

    void SetHP(int hp);

    void SetSP(int sp);

    void SetStamina(int stamina);

    void SetMaxHP(int maxhp);

    void SetMaxSP(int maxsp);

    void SetMaxStamina(int maxstamina);

    int GetHP();

    int GetSP();

    int GetStamina();

    int GetMaxHP();

    int GetMaxSP();

    int GetMaxStamina();

    void BeginFighting(std::shared_ptr<Entity> target);

    void AddAttacker(std::shared_ptr<Entity> attacker);

    std::vector<std::shared_ptr<Entity>> GetAttackers();

    void RemoveAttacker(std::shared_ptr<Entity> attacker);

    void RemoveAllAttackers();

    void StopFighting();

    bool IsFighting();

    bool IsAttacked();

    std::shared_ptr<Entity> GetTarget();

    bool Damage(int amount);

    void Heal(int amount);

    virtual void Send(std::string data);

protected:
    std::string m_name;
    bool m_in_ship;
    int m_planet_id;
    std::weak_ptr<Planet> m_planet;
    std::weak_ptr<Ship> m_ship;
    std::weak_ptr<Room> m_room;
    int m_ship_id;
    int m_room_id;
    int m_max_hp;
    int m_hp;
    int m_max_sp;
    int m_sp;
    int m_max_stamina;
    int m_stamina;
    std::shared_ptr<Entity> m_target;
    std::vector<std::shared_ptr<Entity>> m_attackers;
//    std::map<int, Item> inventory;

private:
};

#endif // ENTITY_H
//
// Created by skelton on 12/22/16.
//

#ifndef SKELMUD_SHIP_H
#define SKELMUD_SHIP_H

#include <vector>
#include <map>
#include "../Areas/Area.h"

class Turret;

//class Area;

class Ship : public Area {
public:
    Ship() : Area(){ };
    Ship(std::string name, int id, int health, int shields) : Area(name, id) {
        m_max_health = health;
        m_max_shields = shields;
        m_hatch = false;
        m_planet_id = 0;
        m_room_id = 0;
    };

    std::string GetShipName();

    void OpenHatch();

    void CloseHatch();

    bool IsHatchOpen();

    void AddCockpit(int room_num);

    void SetPlanetId(int planet_id);

    int GetPlanetId();

    void SetRoomId(int room_id);

    int GetRoomId();

private:
    int m_planet_id;
    int m_room_id;
    int m_max_health;
    int m_max_shields;
    bool m_hatch;
    std::map<int, bool> m_cockpits;
    std::map<int, Turret> m_turrets;
};

class Turret {
private:
    std::string turret_name;
    int m_damage;
    int m_cooldown;
    int m_tracking_strength;
};


#endif //SKELMUD_SHIP_H

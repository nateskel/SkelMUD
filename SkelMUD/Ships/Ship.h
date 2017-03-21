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
    };

    std::string GetShipName();

private:
    int m_max_health;
    int m_max_shields;
    std::vector<int> cockpits;
    std::map<int, Turret> turrets;
};

class Turret {
private:
    std::string turret_name;
    int m_damage;
    int m_cooldown;
    int m_tracking_strength;
};


#endif //SKELMUD_SHIP_H

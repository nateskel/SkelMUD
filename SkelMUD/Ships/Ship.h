//
// Created by skelton on 12/22/16.
//

#ifndef SKELMUD_SHIP_H
#define SKELMUD_SHIP_H

#include <vector>
#include <map>
#include "../Areas/Area.h"
#include "../Utils.h"

class Turret;

class Ship : public Area {
public:
    Ship() : Area(){ };
    Ship(std::string name, int id, int health, int shields) : Area(name, id) {
        m_max_health = health;
        m_max_shields = shields;
        m_hatch = false;
        m_planet_id = 0;
        m_room_id = 0;
        m_in_space = false;
        m_coordinates.x = 0;
        m_coordinates.y = 0;
        m_coordinates.z = 0;
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

    bool IsInSpace();

    void SetInSpace(bool in_space);

    void SetCoordinates(int x, int y, int z);

    void SetCoordinates(Utils::Coordinates coords);

    Utils::Coordinates GetCoordinates();

private:
    int m_planet_id;
    int m_room_id;
    int m_max_health;
    int m_max_shields;
    bool m_hatch;
    bool m_in_space;
    std::map<int, bool> m_cockpits;
    std::map<int, Turret> m_turrets;
    Utils::Coordinates m_coordinates;
};

class Turret {
private:
    std::string turret_name;
    int m_damage;
    int m_cooldown;
    int m_tracking_strength;
};


#endif //SKELMUD_SHIP_H

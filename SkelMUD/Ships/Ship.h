//
// Created by skelton on 12/22/16.
//

#ifndef SKELMUD_SHIP_H
#define SKELMUD_SHIP_H

#include <vector>
#include <map>
#include "../Areas/Area.h"
#include "../Utils.h"
#include "../Connection.h"

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
        m_in_orbit = false;
        m_coordinates.x = 0;
        m_coordinates.y = 0;
        m_coordinates.z = 0;
        m_velocity.x = 0;
        m_velocity.y = 0;
        m_velocity.z = 0;
    };

    std::string GetShipName();

    void OpenHatch();

    void CloseHatch();

    bool IsHatchOpen();

    void AddCockpit(int room_num);

    void SetPlanetId(int planet_id);

    int GetPlanetID();

    void SetRoomId(int room_id);

    int GetRoomId();

    bool IsInSpace();

    void SetInSpace(bool in_space);

    bool IsInOrbit();

    void SetInOrbit(bool in_orbit);

    void SetCoordinates(double x, double y, double z);

    void SetCoordinates(Utils::Vector3 coords);

    Utils::Vector3 GetVelocity();

    void SetVelocity(Utils::Vector3 velocity);

    void SetVelocity(double x, double y, double z);

    Utils::Vector3 GetDestination();

    void SetDestination(Utils::Vector3 destination);

    void SetDestination(double x, double y, double z);

    Utils::Vector3 GetCoordinates();

    void AddPlayer(int player_id);

    void RemovePlayer(int player_id);

    std::vector<int> GetPlayerIDs();

private:
    int m_planet_id;
    int m_room_id;
    int m_max_health;
    int m_max_shields;
    bool m_hatch;
    bool m_in_space;
    bool m_in_orbit;
    Utils::Vector3 m_destination;
    std::map<int, bool> m_cockpits;
    std::map<int, Turret> m_turrets;
    std::vector<int> m_player_ids;
    Utils::Vector3 m_coordinates;
    Utils::Vector3 m_velocity;
};

class Turret {
private:
    std::string turret_name;
    int m_damage;
    int m_cooldown;
    int m_tracking_strength;
};


#endif //SKELMUD_SHIP_H

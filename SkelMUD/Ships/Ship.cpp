//
// Created by skelton on 12/22/16.
//

#include "Ship.h"
#include "../Sender.h"


std::string Ship::GetShipName() {
    return m_area_name;
}

void Ship::OpenHatch() {
    m_hatch = true;
}

void Ship::CloseHatch() {
    m_hatch = false;
}

bool Ship::IsHatchOpen() {
    return m_hatch;
}

void Ship::AddCockpit(int room_num) {
    m_cockpits[room_num] = true;
}

int Ship::GetRoomId() {
    return m_room_id;
}

void Ship::SetRoomId(int room_id) {
    m_room_id = room_id;
}

int Ship::GetPlanetID() {
    return m_planet_id;
}

void Ship::SetPlanetId(int planet_id) {
    m_planet_id = planet_id;
}

bool Ship::IsInSpace() {
    return m_in_space;
}

void Ship::SetInSpace(bool in_space) {
    m_in_space = in_space;
}

bool Ship::IsInOrbit() {
    return m_in_orbit;
}

void Ship::SetInOrbit(bool in_orbit) {
    m_in_orbit = in_orbit;
}

void Ship::SetCoordinates(Utils::Vector3 coords) {
    m_coordinates = coords;
}

void Ship::SetCoordinates(double x, double y, double z) {
    m_coordinates.x = x;
    m_coordinates.y = y;
    m_coordinates.z = z;
}

std::vector<int> Ship::GetPlayerIDs() {
    return m_player_ids;
}

void Ship::AddPlayer(int player_id) {
    m_player_ids.push_back(player_id);
}

void Ship::RemovePlayer(int player_id) {
    std::vector<int>::iterator it = m_player_ids.begin();
    for (auto player : m_player_ids) {
        if(player == player_id) {
            m_player_ids.erase(it);
            return;
        }
        it++;
    }
}

Utils::Vector3 Ship::GetCoordinates() {
    return m_coordinates;
}

void Ship::SetVelocity(Utils::Vector3 velocity) {
    m_velocity = velocity;
}

void Ship::SetVelocity(double x, double y, double z) {
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

Utils::Vector3 Ship::GetVelocity() {
    return m_velocity;
}

void Ship::SetDestination(Utils::Vector3 destination) {
    m_destination = destination;
}

void Ship::SetDestination(double x, double y, double z) {
    m_destination.x = x;
    m_destination.y = y;
    m_destination.z = z;
}

Utils::Vector3 Ship::GetDestination() {
    return m_destination;
}
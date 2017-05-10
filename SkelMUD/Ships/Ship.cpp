//
// Created by skelton on 12/22/16.
//

#include "Ship.h"


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

int Ship::GetPlanetId() {
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

void Ship::SetCoordinates(Utils::Coordinates coords) {
    m_coordinates = coords;
}

void Ship::SetCoordinates(int x, int y, int z) {
    m_coordinates.x = x;
    m_coordinates.y = y;
    m_coordinates.z = z;
}

Utils::Coordinates Ship::GetCoordinates() {
    return m_coordinates;
}

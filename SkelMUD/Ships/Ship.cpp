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


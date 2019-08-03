//
// Created by skelton on 12/22/16.
//

#include <sstream>
#include "Area.h"
#include "../Logger.h"
#include "../Ships/Ships.h"

unsigned long Area::AddRoom(std::shared_ptr<Room> room) {
    m_rooms.push_back(room);
    u_long room_id = m_rooms.size() - 1;
    room->SetID((int)room_id);
    return room_id;
}

void Area::DeleteRoom(int room_id) {
    if(room_id <= m_rooms.size()) {
        m_rooms.erase(m_rooms.begin() + room_id);
    }
}

std::shared_ptr<Room> Area::GetRoom(int id) {
    if(id <= m_rooms.size())
        return m_rooms[id];
    else
        return nullptr;
}

void Area::SetName(std::string name) {
    m_area_name = name;
}

std::string Area::GetName() {
    return m_area_name;
}

int Area::GetID() {
    return m_area_id;
}

bool Area::ChangeRoom(int old_room, int new_room, std::shared_ptr<Player> player) {
    if(new_room >= m_rooms.size())
        return false;
    player->SetRoomID(new_room);
    player->SetRoom(m_rooms[new_room]);
    m_rooms[old_room]->RemovePlayer(player->GetID());
    m_rooms[new_room]->AddPlayer(player);
    return true;
}

std::vector<std::shared_ptr<Room>> Area::GetRooms() {
    return m_rooms;
}


//void Area::PopulateShips(Ships &ships) {
//    for(auto room: m_rooms) {
//        room->PopulateShips(ships, std::shared_ptr<Area>(this));
//    }
//}


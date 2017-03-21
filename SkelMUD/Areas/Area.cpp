//
// Created by skelton on 12/22/16.
//

#include <sstream>
#include "Area.h"
#include "../Logger.h"

unsigned long Area::AddRoom(std::shared_ptr<Room> room) {
    m_rooms.push_back(room);
    u_long room_id = m_rooms.size() - 1;
    room->SetID((int)room_id);
    return room_id;
}

void Area::DeleteRoom(int room_id) {
    Logger::Debug(std::to_string(m_rooms.size()));
    if(room_id <= m_rooms.size()) {
        m_rooms.erase(m_rooms.begin() + room_id);
        Logger::Debug(std::to_string(m_rooms.size()));
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

bool Area::MoveNorth(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetNorth();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveSouth(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetSouth();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveEast(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetEast();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveWest(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetWest();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveNorthWest(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetNorthWest();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveNorthEast(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetNorthEast();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveSouthWest(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetSouthWest();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveSouthEast(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetSouthEast();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveUp(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetUp();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::MoveDown(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetDown();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Area::ChangeRoom(int old_room, int new_room, int player_id) {
    if(new_room >= m_rooms.size())
        return false;
    std::shared_ptr<Player> player = m_rooms[old_room]->GetPlayer(player_id);
    player->SetRoomID(new_room);
    m_rooms[old_room]->RemovePlayer(player_id);
    m_rooms[new_room]->AddPlayer(player);
    return true;
}

std::vector<std::shared_ptr<Room>> Area::GetRooms() {
    return m_rooms;
}




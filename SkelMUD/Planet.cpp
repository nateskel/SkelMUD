#include "Planet.h"

Planet::Planet() {

}

Planet::~Planet() {

}

void Planet::AddRoom(Room* room) {
    m_rooms.push_back(room);
}

Room* Planet::GetRoom(int id) {
    return m_rooms[id];
}

void Planet::SetName(std::string name) {
    m_planet_name = name;
}

bool Planet::MoveNorth(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetNorth();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveSouth(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetSouth();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveEast(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetEast();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveWest(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetWest();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveNorthWest(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetNorthWest();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveNorthEast(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetNorthEast();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveSouthWest(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetSouthWest();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveSouthEast(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetSouthEast();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveUp(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetUp();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

bool Planet::MoveDown(int room_id, int player_id) {
    int new_room = m_rooms[room_id]->GetDown();
    if (new_room == -1)
        return false;
    ChangeRoom(room_id, new_room, player_id);
    return true;
}

void Planet::ChangeRoom(int old_room, int new_room, int player_id) {
    Player* player = m_rooms[old_room]->GetPlayer(player_id);
    player->SetRoomID(new_room);
    m_rooms[old_room]->RemovePlayer(player_id);
    m_rooms[new_room]->AddPlayer(player);
}

Room::Room() {

}

Room::Room(std::string long_desc,
           std::string short_desc,
           int n,
           int s,
           int e,
           int w,
           int ne,
           int nw,
           int se,
           int sw,
           int u,
           int d) {
    m_long_description = long_desc;
    m_short_description = short_desc;
    m_north = n;
    m_south = s;
    m_east = e;
    m_west = w;
    m_northeast = ne;
    m_northwest = nw;
    m_southeast = se;
    m_southwest = sw;
    m_up = u;
    m_down = d;
}

Room::~Room() {

}

void Room::AddPlayer(Player* player) {
    m_player_map[player->GetID()] = player;
}

Player* Room::GetPlayer(int id) {
    return m_player_map[id];
}

void Room::RemovePlayer(int id) {
    m_player_map.erase(id);
}

std::vector<int> Room::GetPlayers() {
    std::vector<int> output;
    for (std::map<int, Player*>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        output.push_back(it->first);
    }
    return output;
}

std::vector<int> Room::GetPlayerIDs(int exclude) {
    std::vector<int> output;
    for (std::map<int, Player*>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        if (it->first != exclude)
            output.push_back(it->first);
    }
    return output;
}

std::string Room::GetLongDescription() {
    return m_long_description;
}

std::string Room::GetShortDescription() {
    return m_short_description;
}

int Room::GetNorth() {
    return m_north;
}

int Room::GetSouth() {
    return m_south;
}

int Room::GetEast() {
    return m_east;
}

int Room::GetWest() {
    return m_west;
}

int Room::GetNorthWest() {
    return m_northwest;
}

int Room::GetNorthEast() {
    return m_northeast;
}

int Room::GetSouthWest() {
    return m_southwest;
}

int Room::GetSouthEast() {
    return m_southeast;
}

int Room::GetUp() {
    return m_up;
}

int Room::GetDown() {
    return m_down;
}
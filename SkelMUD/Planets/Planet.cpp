#include <sstream>
#include "Planet.h"
#include "../Logger.h"

unsigned long Planet::AddRoom(std::shared_ptr<Room> room) {
    m_rooms.push_back(room);
    u_long room_id = m_rooms.size() - 1;
    room->SetID((int)room_id);
    return room_id;
}

void Planet::DeleteRoom(int room_id) {
    Logger::Debug(std::to_string(m_rooms.size()));
    if(room_id <= m_rooms.size()) {
        m_rooms.erase(m_rooms.begin() + room_id);
        Logger::Debug(std::to_string(m_rooms.size()));
    }
}

std::shared_ptr<Room> Planet::GetRoom(int id) {
    if(id <= m_rooms.size())
        return m_rooms[id];
    else
        return nullptr;
}

void Planet::SetName(std::string name) {
    m_planet_name = name;
}

std::string Planet::GetName() {
    return m_planet_name;
}

int Planet::GetID() {
    return m_planet_id;
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

bool Planet::ChangeRoom(int old_room, int new_room, int player_id) {
    if(new_room >= m_rooms.size())
        return false;
    std::shared_ptr<Player> player = m_rooms[old_room]->GetPlayer(player_id);
    player->SetRoomID(new_room);
    m_rooms[old_room]->RemovePlayer(player_id);
    m_rooms[new_room]->AddPlayer(player);
    return true;
}

std::vector<std::shared_ptr<Room>> Planet::GetRooms() {
    return m_rooms;
}

Room::Room() {
    m_id = 0;
    m_long_description = "Basic Room <Long>";
    m_short_description = "Basic Room <Short>";
    m_north = -1;
    m_south = -1;
    m_east = -1;
    m_west = -1;
    m_northeast = -1;
    m_northwest = -1;
    m_southeast = -1;
    m_southwest = -1;
    m_up = -1;
    m_down = -1;
}

Room::Room(std::string long_desc, std::string short_desc, int n, int s, int e, int w, int ne, int nw, int se,
           int sw, int u, int d) {
    m_id = 0;
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

void Room::AddPlayer(std::shared_ptr<Player> player) {
    m_player_map[player->GetID()] = player;
}

std::shared_ptr<Player> Room::GetPlayer(int id) {
    return m_player_map[id];
}

void Room::RemovePlayer(int id) {
    m_player_map.erase(id);
}

std::vector<int> Room::GetVisiblePlayers() {
    std::vector<int> output;
    for (std::map<int, std::shared_ptr<Player>>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        if(it->second->IsVisible())
            output.push_back(it->first);
    }
    return output;
}

std::vector<int> Room::GetVisiblePlayers(int exclude) {
    std::vector<int> output;
    for (std::map<int, std::shared_ptr<Player>>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        if (it->first != exclude and it->second->IsVisible())
            output.push_back(it->first);
    }
    return output;
}

std::vector<int> Room::GetPlayers() {
    std::vector<int> output;
    for (std::map<int, std::shared_ptr<Player>>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        output.push_back(it->first);
    }
    return output;
}

std::vector<int> Room::GetPlayers(int exclude) {
    std::vector<int> output;
    for (std::map<int, std::shared_ptr<Player>>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        if (it->first != exclude)
            output.push_back(it->first);
    }
    return output;
}

std::vector<std::string> Room::GetVisiblePlayerNames(int exclude) {
    std::vector<std::string> output;
    for (std::map<int, std::shared_ptr<Player>>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        if (it->first != exclude and it->second->IsVisible())
            output.push_back(it->second->GetPlayerName());
    }
    return output;
}

std::vector<std::string> Room::GetPlayerNames(int exclude) {
    std::vector<std::string> output;
    for (std::map<int, std::shared_ptr<Player>>::iterator it = m_player_map.begin(); it != m_player_map.end(); it++) {
        if (it->first != exclude)
            output.push_back(it->second->GetPlayerName());
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


void Room::SetNorth(int north) {
    m_north = north;
}

void Room::SetSouth(int south) {
    m_south = south;
}

void Room::SetEast(int east) {
    m_east = east;
}

void Room::SetWest(int west) {
    m_west = west;
}

void Room::SetNortheast(int ne) {
    m_northeast = ne;
}

void Room::SetNorthwest(int nw) {
    m_northwest = nw;
}

void Room::SetSoutheast(int se) {
    m_southeast = se;
}

void Room::SetSouthwest(int sw) {
    m_southwest = sw;
}

void Room::SetUp(int up) {
    m_up = up;
}

void Room::SetDown(int down) {
    m_down = down;
}


int Room::GetID() {
    return m_id;
}

void Room::SetID(int id) {
    m_id = id;
}


void Room::SetLongDesc(std::string long_desc) {
    m_long_description = long_desc;
}

void Room::SetShortDesc(std::string short_desc) {
    m_short_description = short_desc;
}




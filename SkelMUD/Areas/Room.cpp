//
// Created by nate on 3/19/17.
//

#include <sstream>
#include "Room.h"
#include "../Ships/Ship.h"
#include "../Logger.h"
#include "../GameData.h"

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
    m_landing_level = NONE;
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
    m_landing_level = NONE;
}

Room::~Room() {

}

void Room::AddShipID(int ship_id) {
    m_ship_ids.push_back(ship_id);
}


void Room::PopulateShips(Ships &ships) {
    auto all_ships = ships.GetShips();
    for(auto ship_id: m_ship_ids) {
        m_ships[ship_id] = all_ships[ship_id];
    }
}


std::vector<int> Room::GetShipIDs() {
    return m_ship_ids;
}


std::map<int, std::shared_ptr<Ship>> Room::GetShips() {
    return m_ships;
}


void Room::AddShipIDs(std::vector<int> ship_ids) {
    m_ship_ids.insert(m_ship_ids.end(), ship_ids.begin(), ship_ids.end());
}


void Room::RemoveShip(int id) {

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

void Room::SetLandingLevel(Room::LandingLevel landing_level) {
    m_landing_level = landing_level;
}

Room::LandingLevel Room::GetLandingLevel() {
    return m_landing_level;
}

bool Room::IsLandable() {
    return m_landing_level != LandingLevel::NONE;
}
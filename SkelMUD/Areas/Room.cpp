//
// Created by nate on 3/19/17.
//

#include "Room.h"
#include "../Ships/Ship.h"
#include "../Logger.h"
#include "../GameData.h"

Room::Room() {
    m_id = 0;
    m_long_description = "Basic Room <Long>";
    m_short_description = "Basic Room <Short>";
    for(int& m_direction : m_directions) {
        m_direction = -1;
    }
    m_landing_level = NONE;
    m_is_cockpit = false;
}

Room::Room(const std::string &long_desc, const std::string &short_desc, int n, int s, int e, int w, int ne, int nw,
           int se, int sw, int u, int d) {
    m_id = 0;
    m_long_description = long_desc;
    m_short_description = short_desc;
    m_directions[NORTH] = n;
    m_directions[SOUTH] = s;
    m_directions[EAST] = e;
    m_directions[WEST] = w;
    m_directions[NORTHEAST] = ne;
    m_directions[NORTHWEST] = nw;
    m_directions[SOUTHEAST] = se;
    m_directions[SOUTHWEST] = sw;
    m_directions[UP] = u;
    m_directions[DOWN] = d;
    m_landing_level = NONE;
    m_is_cockpit = false;
}

Room::Room(const std::string &long_desc, const std::string &short_desc, const int *directions) {
    m_id = 0;
    m_long_description = long_desc;
    m_short_description = short_desc;
    for(int i = 0; i < NUMBER_OF_DIRECTIONS; ++i) {
        m_directions[i] = directions[i];
    }
    m_landing_level = NONE;
    m_is_cockpit = false;
}

Room::~Room() = default;

void Room::SetIsCockpit(bool is_cockpit) {
    m_is_cockpit = is_cockpit;
}

bool Room::IsCockpit() const {
    return m_is_cockpit;
}

void Room::AddShipID(int ship_id) {
    m_ship_ids.push_back(ship_id);
}

void Room::AddShip(const std::shared_ptr<Ship>& ship) {
    int id = ship->GetID();
    AddShipID(id);
    m_ships[id] = ship;
}

//void Room::PopulateShips(Ships &ships, std::shared_ptr<Area> area) {
//    auto all_ships = ships.GetShips();
//    for(auto ship_id: m_ship_ids) {
//        auto ship = all_ships[ship_id];
//        //ship->SetContainingRoom(&this);
//        m_ships[ship_id] = ship;
//    }
//}

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
    m_ships.erase(id);
}

void Room::AddPlayer(const std::shared_ptr<Player>& player) {
    m_player_map[player->GetID()] = player;
}

void Room::AddNPC(const std::shared_ptr<NPC>& npc) {
    m_npc_map[npc->GetName()] = npc;
}

std::vector<std::string> Room::GetNPCs() {
    std::vector<std::string> output;
    for (auto & it : m_npc_map) {
        output.push_back(it.first);
    }
    return output;
}

std::shared_ptr<Player> Room::GetPlayer(int id) {
    return m_player_map[id];
}

void Room::RemovePlayer(int id) {
    m_player_map.erase(id);
}

std::vector<int> Room::GetVisiblePlayers() {
    std::vector<int> output;
    for (auto & it : m_player_map) {
        if(it.second->IsVisible())
            output.push_back(it.first);
    }
    return output;
}

std::vector<int> Room::GetVisiblePlayers(int exclude) {
    std::vector<int> output;
    for (auto & it : m_player_map) {
        if (it.first != exclude and it.second->IsVisible())
            output.push_back(it.first);
    }
    return output;
}

std::vector<int> Room::GetPlayers() {
    std::vector<int> output;
    for (auto & it : m_player_map) {
        output.push_back(it.first);
    }
    return output;
}

std::vector<int> Room::GetPlayers(int exclude) {
    std::vector<int> output;
    for (auto & it : m_player_map) {
        if (it.first != exclude)
            output.push_back(it.first);
    }
    return output;
}

std::vector<std::string> Room::GetVisiblePlayerNames(int exclude) {
    std::vector<std::string> output;
    for (auto & it : m_player_map) {
        if (it.first != exclude and it.second->IsVisible())
            output.push_back(it.second->GetPlayerName());
    }
    return output;
}

std::vector<std::string> Room::GetVisiblePlayerNames(const std::string& exclude_name) {
    std::vector<std::string> output;
    for (auto & it : m_player_map) {
        if (it.second->GetPlayerName() != exclude_name and it.second->IsVisible())
            output.push_back(it.second->GetPlayerName());
    }
    return output;
}

std::vector<std::string> Room::GetPlayerNames(int exclude) {
    std::vector<std::string> output;
    for (auto & it : m_player_map) {
        if (it.first != exclude)
            output.push_back(it.second->GetPlayerName());
    }
    return output;
}

std::string Room::GetLongDescription() {
    return m_long_description;
}

std::string Room::GetShortDescription() {
    return m_short_description;
}

int Room::GetDirection(Room::Directions direction) {
    return m_directions[direction];
}

int Room::GetNorth() {
    return GetDirection(NORTH);
}

int Room::GetSouth() {
    return GetDirection(SOUTH);
}

int Room::GetEast() {
    return GetDirection(EAST);
}

int Room::GetWest() {
    return GetDirection(WEST);
}

int Room::GetNorthWest() {
    return GetDirection(NORTHWEST);
}

int Room::GetNorthEast() {
    return GetDirection(NORTHEAST);
}

int Room::GetSouthWest() {
    return GetDirection(SOUTHWEST);
}

int Room::GetSouthEast() {
    return GetDirection(SOUTHEAST);
}

int Room::GetUp() {
    return GetDirection(UP);
}

int Room::GetDown() {
    return GetDirection(DOWN);
}

void Room::SetNorth(int north) {
    m_directions[NORTH] = north;
}

void Room::SetSouth(int south) {
    m_directions[SOUTH] = south;
}

void Room::SetEast(int east) {
    m_directions[EAST] = east;
}

void Room::SetWest(int west) {
    m_directions[WEST] = west;
}

void Room::SetNortheast(int ne) {
    m_directions[NORTHEAST] = ne;
}

void Room::SetNorthwest(int nw) {
    m_directions[NORTHWEST] = nw;
}

void Room::SetSoutheast(int se) {
    m_directions[SOUTHEAST] = se;
}

void Room::SetSouthwest(int sw) {
    m_directions[SOUTHWEST] = sw;
}

void Room::SetUp(int up) {
    m_directions[UP] = up;
}

void Room::SetDown(int down) {
    m_directions[DOWN] = down;
}

int Room::GetID() const {
    return m_id;
}

void Room::SetID(int id) {
    m_id = id;
}


void Room::SetLongDesc(const std::string &long_desc) {
    m_long_description = long_desc;
}

void Room::SetShortDesc(const std::string &short_desc) {
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

void Room::AddItem(const std::string& item) {
    if(m_items.find(item) == m_items.end()) {
        m_items[item] = 1;
    }
    else {
        m_items[item]++;
    }
}

void Room::RemoveItem(const std::string& item) {
    if(m_items.find(item) != m_items.end()) {
        m_items[item]--;
        if(m_items[item] == 0) {
            m_items.erase(item);
        }
    }
}

std::map<std::string, int> Room::GetItems() {
    return m_items;
}
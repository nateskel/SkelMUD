//
// Created by nate on 11/18/15.
//

#include "Connection.h"
#include "GameData.h"
#include "Logger.h"

const std::string GameData::BASE_PATH = "/mnt/c/users/skelt/clionprojects/skelmud/skelmud/data/";
const std::string GameData::ACCOUNT_FILE = BASE_PATH + "Accounts.sml";
const std::string GameData::RACE_FILE = BASE_PATH + "Races.sml";
const std::string GameData::CLASS_FILE = BASE_PATH + "Classes.sml";
const std::string GameData::CHARACTER_FILE = BASE_PATH + "Characters.sml";
const std::string GameData::PLANET_PATH = BASE_PATH + "Planets/";
const std::string GameData::PLANET_FILE = PLANET_PATH + "Planets.sml";
const std::string GameData::SHIP_PATH = BASE_PATH + "Ships/";
const std::string GameData::SHIP_FILE = SHIP_PATH + "Ships.sml";
const std::string GameData::CONFIG_FILE = BASE_PATH + "config.sml";
const std::string GameData::ACCOUNT_DATA = BASE_PATH + "AccountData/";
const std::string GameData::ITEM_PATH = BASE_PATH + "Items/";
const std::string GameData::NPC_PATH = BASE_PATH + "NPCs/";


void GameData::AddConnection(std::shared_ptr<Connection> connection) {
    m_connections[connection->GetSocket()] = connection;
}

std::shared_ptr<Connection> GameData::GetConnection(int index) {
    return m_connections[index];
}

std::map<int, std::shared_ptr<Connection>> GameData::GetAllConnections() {
    return m_connections;
}

std::map<int, std::shared_ptr<Connection>> GameData::GetConnections(std::vector<int> connection_ids) {
    std::map<int, std::shared_ptr<Connection>> connection_map;
    for(auto connection : connection_ids) {
        connection_map[connection] = GetConnection(connection);
    }
    return connection_map;
}

std::map<int, std::shared_ptr<Connection>> GameData::GetLoggedInConnections() {
    std::map<int, std::shared_ptr<Connection>> connection_map;
    for(auto connection : m_connections)
    {
        if(connection.second->IsLoggedIn())
            connection_map.insert(connection);
    }
    return connection_map;
};

void GameData::EraseConnection(int index) {
    m_connections.erase(index);
}

GameData::GameData() {
    Logger::Debug("Loading Resources");
    m_accounts.LoadAccounts(ACCOUNT_FILE);
    Logger::Debug("Accounts Loaded");
    m_races.LoadRaces(RACE_FILE);
    Logger::Debug("Races Loaded");
    m_classes.LoadClasses(CLASS_FILE);
    Logger::Debug("Classes Loaded");
    m_characters.LoadCharacters(CHARACTER_FILE);
    Logger::Debug("Characters Loaded");
    m_planets.LoadPlanets(PLANET_FILE);
    Logger::Debug("Planets Loaded");
    m_ships.LoadShips(SHIP_FILE);
    Logger::Debug("Ships Loaded");
    m_items.LoadItems(ITEM_PATH);
    Logger::Debug("Items Loaded");
    m_npcs.LoadNPCs(NPC_PATH);
    Logger::Debug("NPCs Loaded");
    m_configuration.LoadConfig(CONFIG_FILE);
    PopulateShips();
    PopulateNPCs();
    PopulateRaces();
    auto room = GetRoom(0, 1, false);
    for(const auto& item : m_items.GetItems()) {
        room->AddItem(item.first);
        room->AddItem(item.first);
    }
    Logger::Debug("Resources Loaded");
}

Accounts GameData::GetAccounts() {
    return m_accounts;
}

Account GameData::GetAccount(const std::string& username) {
    return m_accounts.GetAccount(username);
}

void GameData::AddAccount(Account account) {
    m_accounts.AddAccount(account);
}

void GameData::SaveAccounts(std::string filename) {
    m_accounts.SaveAccounts(filename);
}

void GameData::SaveCharacters(std::string filename) {
    m_characters.SaveCharacters(filename);
}

Races GameData::GetRaces() {
    return m_races;
}

std::shared_ptr<Race> GameData::GetRace(std::string race_name) {
    return m_races.GetRaces()[race_name];
}

CharacterClasses GameData::GetClasses() {
    return m_classes;
}

void GameData::AddCharacter(std::string username, std::shared_ptr<Player> player) {
    m_characters.AddCharacter(player);
    m_accounts.AddCharacter(username, player->GetPlayerName());
}

std::shared_ptr<Player> GameData::GetPlayer(std::string name) {
    return m_characters.GetCharacters()[name];
}

std::map<std::string, std::shared_ptr<Player>> GameData::GetPlayers() {
    return m_characters.GetCharacters();
}

std::shared_ptr<Connection> GameData::GetConnection(std::string character_name) {
    for(auto connection: m_connections) {
        if(connection.second->GetCharacterName() == character_name and connection.second->IsLoggedIn()) {
            return connection.second;
        }
    }
    return nullptr;
}

std::shared_ptr<Planet> GameData::GetPlanet(int ID) {
    return m_planets.GetPlanets()[ID];
}
std::shared_ptr<Room> GameData::GetRoom(int AreaID, int RoomID, bool is_ship) {
    return GetArea(AreaID, is_ship)->GetRoom(RoomID);
}

std::shared_ptr<Area> GameData::GetArea(int ID, bool is_ship) {
    if(is_ship) {
        return GetShip(ID);
    }
    else {
        return GetPlanet(ID);
    }
}

std::shared_ptr<Planet> GameData::GetPlanet(std::string name) {
    for(auto planet : m_planets.GetPlanets()) {
        if(planet.second->GetName() == name) {
            return planet.second;
        }
    }
    return nullptr;
}

Planets& GameData::GetPlanets() {
    return m_planets;
}

void GameData::SavePlanet(int id) {
    m_planets.SavePlanet(id);
}

Ships& GameData::GetShips() {
    return m_ships;
}

std::shared_ptr<Ship> GameData::GetShip(int id) {
    return m_ships.GetShips()[id];
}

void GameData::SaveShip(int id) {
    m_ships.SaveShip(id);
}

std::shared_ptr<Item> GameData::GetItem(std::string name) {
    return m_items.GetItem(name);
}

std::shared_ptr<NPC> GameData::GetNPC(std::string name) {
    return m_npcs.GetNPCs()[name];
}

Configuration& GameData::GetConfiguration() {
    return m_configuration;
}

void GameData::PopulateShips() {
    for (auto planet : m_planets.GetPlanets()) {
        for(auto room : planet.second->GetRooms()) {
            for(int ship_id : room->GetShipIDs()) {
                auto ship = this->GetShip(ship_id);
                room->AddShip(ship);
                ship->SetContainingRoom(room);
                ship->SetPlanet(planet.second);
            }
        }
    }
}

void GameData::PopulateNPCs() {
    for (auto npc : m_npcs.GetNPCs()) {
        auto area = GetArea(npc.second->GetLocationID(), npc.second->IsInShip());
        auto room = area->GetRoom(npc.second->GetRoomID());
        npc.second->SetRoom(room);
        room->AddNPC(npc.second);
    }
}

void GameData::PopulateRaces() {
    for (auto entity : m_characters.GetCharacters()) {
        auto player = entity.second;
        player->SetPlayerRace(GetRace(player->GetPlayerRaceStr()));
    }
}

std::shared_ptr<Connection> GameData::GetConnectionByUsername(std::string username) {
    for (auto connection : GetLoggedInConnections()) {
        if (connection.second->GetUsername() == username)
            return connection.second;
    }
    return nullptr;
}

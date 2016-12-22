//
// Created by nate on 11/18/15.
//

#include "GameData.h"
#include "Logger.h"

const std::string GameData::BASE_PATH = "/home/skelton/SkelMUD/SkelMUD/SkelMUD/";
const std::string GameData::ACCOUNT_FILE = BASE_PATH + "Accounts/Accounts.sml";
const std::string GameData::RACE_FILE = BASE_PATH + "Races/Races.sml";
const std::string GameData::CLASS_FILE = BASE_PATH + "Classes/Classes.sml";
const std::string GameData::CHARACTER_FILE = BASE_PATH + "Characters/Characters.sml";
const std::string GameData::PLANET_PATH = BASE_PATH + "Planets/";
const std::string GameData::PLANET_FILE = PLANET_PATH + "Planets.sml";
const std::string GameData::CONFIG_FILE = BASE_PATH + "Config/config.sml";


void GameData::AddConnection(std::shared_ptr<Connection> connection) {
    m_connections[connection->GetSocket()] = connection;
}

std::shared_ptr<Connection> GameData::GetConnection(int index) {
    return m_connections[index];
}

std::map<int, std::shared_ptr<Connection>> GameData::GetAllConnections() {
    return m_connections;
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
    m_races.LoadRaces(RACE_FILE);
    m_classes.LoadClasses(CLASS_FILE);
    m_characters.LoadCharacters(CHARACTER_FILE);
    m_planets.LoadPlanets(PLANET_FILE);
    m_configuration.LoadConfig(CONFIG_FILE);
    Logger::Debug("Resources Loaded");
}

Accounts GameData::GetAccounts() {
    return m_accounts;
}

Account GameData::GetAccount(std::string username) {
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

std::shared_ptr<Connection> GameData::GetConnection(std::string character_name) {
    for(auto connection: m_connections) {
        if(connection.second->GetCharacterName() == character_name) {
            return connection.second;
        }
    }
    return nullptr;
}

std::shared_ptr<Planet> GameData::GetPlanet(int ID) {
    return m_planets.GetPlanets()[ID];
}

std::shared_ptr<Planet> GameData::GetPlanet(std::string name) {
    //return m_planets.GetPlanets()[name];
    return nullptr;
}

Planets& GameData::GetPlanets() {
    return m_planets;
}

void GameData::SavePlanet(int id) {
    m_planets.SavePlanet(id);
}

Configuration& GameData::GetConfiguration() {
    return m_configuration;
}
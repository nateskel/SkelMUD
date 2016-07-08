//
// Created by nate on 11/18/15.
//

#include "GameData.h"
#include "Logger.h"

void GameData::AddConnection(std::shared_ptr<Connection> connection) {
    m_connections[connection->GetID()] = connection;

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
    m_accounts.LoadAccounts(ACCOUNT_FILE);
    m_races.LoadRaces(RACE_FILE);
    m_classes.LoadClasses(CLASS_FILE);
}

Accounts GameData::GetAccounts() {
    return m_accounts;
}

void GameData::AddAccount(Account account) {
    m_accounts.AddAccount(account);
}

void GameData::SaveAccounts(std::string filename) {
    m_accounts.SaveAccounts(filename);
}


Races GameData::GetRaces() {
    return m_races;
}


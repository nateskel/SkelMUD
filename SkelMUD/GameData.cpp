//
// Created by nate on 11/18/15.
//

#include "GameData.h"
#include "Logger.h"

void GameData::AddConnection(std::shared_ptr<Connection> connection) {
    connections[connection->GetID()] = connection;

}

std::shared_ptr<Connection> GameData::GetConnection(int index) {
    return connections[index];
}

std::map<int, std::shared_ptr<Connection>> GameData::GetAllConnections() {
    return connections;
}

std::map<int, std::shared_ptr<Connection>> GameData::GetLoggedInConnections() {
    std::map<int, std::shared_ptr<Connection>> connection_map;
    for(auto connection : connections)
    {
        if(connection.second->IsLoggedIn())
            connection_map.insert(connection);
    }
    return connection_map;
};

void GameData::EraseConnection(int index) {
    connections.erase(index);
}

GameData::GameData() {
    _accounts.LoadAccounts(ACCOUNT_FILE);
}

Accounts GameData::GetAccounts() {
    return _accounts;
}

void GameData::AddAccount(Account account) {
    _accounts.AddAccount(account);
}

void GameData::SaveAccounts(std::string filename) {
    _accounts.SaveAccounts(filename);
}

//
// Created by nate on 11/18/15.
//

#include "GameData.h"

void GameData::AddConnection(std::shared_ptr<Connection> connection) {
    connections[connection->GetSocket()] = connection;

}

std::shared_ptr<Connection> GameData::GetConnection(int index) {
    return connections[index];
}

std::map<int, std::shared_ptr<Connection>> GameData::GetAllConnections() {
    return connections;
}

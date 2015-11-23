//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_GAMEDATA_H
#define SKELMUD_GAMEDATA_H


#include <map>
#include "Connection.h"

class GameData {
public:
    void AddConnection(std::shared_ptr<Connection> connection);
    std::shared_ptr<Connection> GetConnection(int index);
    std::map<int, std::shared_ptr<Connection>> GetAllConnections();
    void EraseConnection(int index);

private:
    std::map<int, std::shared_ptr<Connection>> connections;
};


#endif //SKELMUD_GAMEDATA_H

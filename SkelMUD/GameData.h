//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_GAMEDATA_H
#define SKELMUD_GAMEDATA_H


#include <map>
#include "Connection.h"
#include "Accounts.h"

#define ACCOUNT_FILE "/home/skelton/SkelMUD/SkelMUD/SkelMUD/Accounts.sml"

class GameData {
public:
    GameData();
    void AddConnection(std::shared_ptr<Connection> connection);
    std::shared_ptr<Connection> GetConnection(int index);
    std::map<int, std::shared_ptr<Connection>> GetAllConnections();
    void EraseConnection(int index);
    Accounts GetAccounts();

private:
    std::map<int, std::shared_ptr<Connection>> connections;
    Accounts _accounts;
};


#endif //SKELMUD_GAMEDATA_H

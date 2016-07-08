//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_GAMEDATA_H
#define SKELMUD_GAMEDATA_H


#include <map>
#include "Connection.h"
#include "Accounts/Accounts.h"
#include "Accounts/Account.h"
#include "Races/Races.h"
#include "Classes/CharacterClasses.h"

#define ACCOUNT_FILE "/home/nate/SkelMUD/SkelMUD/Accounts/Accounts.sml"
#define RACE_FILE "/home/nate/SkelMUD/SkelMUD/Races/Races.sml"
#define CLASS_FILE "/home/nate/SkelMUD/SkelMUD/Classes/Classes.sml"

class GameData {
public:
    GameData();
    void AddConnection(std::shared_ptr<Connection> connection);
    std::shared_ptr<Connection> GetConnection(int index);
    std::map<int, std::shared_ptr<Connection>> GetAllConnections();
    void EraseConnection(int index);
    Accounts GetAccounts();
    Races GetRaces();
    void AddAccount(Account account);
    void SaveAccounts(std::string filename);
    std::map<int, std::shared_ptr<Connection>> GetLoggedInConnections();

private:
    std::map<int, std::shared_ptr<Connection>> m_connections;
    Accounts m_accounts;
    Races m_races;
    CharacterClasses m_classes;
};


#endif //SKELMUD_GAMEDATA_H

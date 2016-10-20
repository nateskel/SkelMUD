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
#include "Characters/Characters.h"

class GameData {

public:
    static const std::string ACCOUNT_FILE;
    static const std::string RACE_FILE;
    static const std::string CLASS_FILE;
    static const std::string CHARACTER_FILE;

    GameData();
    void AddConnection(std::shared_ptr<Connection> connection);
    std::shared_ptr<Connection> GetConnection(int index);
    std::map<int, std::shared_ptr<Connection>> GetAllConnections();
    void EraseConnection(int index);
    Accounts GetAccounts();
    Account GetAccount(std::string username);
    void AddCharacter(std::string username, std::string character);
    Races GetRaces();
    CharacterClasses GetClasses();
    void AddAccount(Account account);
    void SaveAccounts(std::string filename);
    std::map<int, std::shared_ptr<Connection>> GetLoggedInConnections();

private:
    std::map<int, std::shared_ptr<Connection>> m_connections;
    Accounts m_accounts;
    Races m_races;
    CharacterClasses m_classes;
    Characters m_characters;
};


#endif //SKELMUD_GAMEDATA_H

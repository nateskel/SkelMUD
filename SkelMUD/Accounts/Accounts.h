//
// Created by skelton on 3/17/16.
//

#ifndef SKELMUD_ACCOUNTS_H
#define SKELMUD_ACCOUNTS_H


#include <map>
#include <vector>
#include "Account.h"

class Accounts {
private:
    std::map<std::string, Account> m_accounts;
    std::vector<std::string> m_characters;
public:
    void AddAccount(Account account);
    void AddAccount(std::string username, std::string password, int account_level);
    void AddAccount(std::string username, size_t password, int account_level);
    void AddCharacter(std::string username, std::string character_name);
    void LoadAccounts(std::string filename);
    void SaveAccounts(std::string filename);
    bool IsAccount(std::string username);
    Account GetAccount(const std::string& username);
};

#endif //SKELMUD_ACCOUNTS_H

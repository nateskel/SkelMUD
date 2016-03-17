//
// Created by skelton on 3/17/16.
//

#ifndef SKELMUD_ACCOUNTS_H
#define SKELMUD_ACCOUNTS_H


#include <map>
#include "Account.h"

class Accounts {
private:
    std::map<std::string, Account> _accounts;
public:
    void AddAccount(Account account);
    void AddAccount(std::string username, std::string password);
    void AddAccount(std::string username, size_t password);
    void LoadAccounts(std::string filename);
    void SaveAccounts(std::string filename);
    bool IsAccount(std::string username);
    Account GetAccount(std::string username);
};


#endif //SKELMUD_ACCOUNTS_H

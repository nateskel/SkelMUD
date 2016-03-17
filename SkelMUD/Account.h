//
// Created by skelton on 3/17/16.
//

#ifndef SKELMUD_ACCOUNT_H
#define SKELMUD_ACCOUNT_H


#include <string>

class Account {
private:
    std::string _username;
    size_t _password;
public:
    Account();
    Account(const std::string &username, const std::string &password);
    Account(const std::string &username, const size_t &password);
    void SetUsername(std::string username);
    void SetPassword(std::string password);
    std::string GetUsername();
    size_t GetPassword();
    bool MatchPassword(std::string password);
};


#endif //SKELMUD_ACCOUNT_H

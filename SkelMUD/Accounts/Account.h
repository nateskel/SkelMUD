//
// Created by skelton on 3/17/16.
//

#ifndef SKELMUD_ACCOUNT_H
#define SKELMUD_ACCOUNT_H


#include <string>

class Account {
private:
    std::string _username;
    int _account_level;
    size_t _password;
public:
    static const int RESTRICTED = 0;
    static const int TRIAL = 1;
    static const int NORMAL = 2;
    static const int WIZARD = 3;
    static const int ADMIN = 4;

    Account();
    Account(const std::string &username, const std::string &password, int account_level);
    Account(const std::string &username, const size_t &password, int account_level);
    void SetUsername(std::string username);
    void SetPassword(std::string password);
    void SetAccountLevel(int level);
    std::string GetUsername();
    size_t GetPassword();
    int GetAccountLevel();
    bool MatchPassword(std::string password);
    std::string GetCharacterList();
};


#endif //SKELMUD_ACCOUNT_H

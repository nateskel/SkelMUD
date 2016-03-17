//
// Created by skelton on 3/17/16.
//

#include "Account.h"
#include "Logger.h"

void Account::SetUsername(std::string username) {
    _username = username;
}

void Account::SetPassword(std::string password) {
    std::hash<std::string> string_hash;
    size_t hashed_password = string_hash(password);
    Logger::Info(std::to_string(hashed_password));
    _password = hashed_password;
}

std::string Account::GetUsername() {
    return _username;
}

bool Account::MatchPassword(std::string password) {
    std::hash<std::string> string_hash;
    size_t hashed_password = string_hash(password);
    return hashed_password == _password;
}

Account::Account() {
    _username = "";
    _password = 0;
}

Account::Account(const std::string &username, const std::string &password) {
    _username = username;
    SetPassword(password);
}

Account::Account(const std::string &username, const size_t &password) {
    _username = username;
    _password = password;
}

size_t Account::GetPassword() {
    return _password;
}

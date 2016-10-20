//
// Created by skelton on 3/17/16.
//

#include "Account.h"
#include "../Logger.h"

void Account::SetUsername(std::string username) {
    _username = username;
}

void Account::SetPassword(std::string password) {
    std::hash<std::string> string_hash;
    size_t hashed_password = string_hash(password);
    Logger::Info(std::to_string(hashed_password));
    _password = hashed_password;
}

void Account::SetAccountLevel(int level) {
    _account_level = level;
}

int Account::GetAccountLevel() {
    return _account_level;
}

std::string Account::GetUsername() {
    return _username;
}

bool Account::MatchPassword(std::string password) {
    std::hash<std::string> string_hash;
    size_t hashed_password = string_hash(password);
    return hashed_password == _password;
}

std::string Account::GetCharacterList() {
    return "";
}

Account::Account() {
    _username = "";
    _password = 0;
    _account_level = NORMAL;
}

Account::Account(const std::string &username, const std::string &password, int account_level) {
    _username = username;
    SetPassword(password);
    _account_level = account_level;
}

Account::Account(const std::string &username, const size_t &password, int account_level) {
    _username = username;
    _password = password;
    _account_level = account_level;
}

size_t Account::GetPassword() {
    return _password;
}

void Account::AddCharacter(std::string character) {
    m_characters.push_back(character);
}

std::vector<std::string> Account::GetCharacters() {
    return m_characters;
}
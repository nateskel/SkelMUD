//
// Created by skelton on 3/17/16.
//

#include "Accounts.h"
#include "../Skexml/SkexmlParser.h"

void Accounts::AddAccount(Account account) {
    _accounts[account.GetUsername()] = account;
}

void Accounts::AddAccount(std::string username, std::string password, int account_level) {
    Account account(username, password, account_level);
    _accounts[username] = account;
}

void Accounts::AddAccount(std::string username, size_t password, int account_level) {
    Account account(username, password, account_level);
    _accounts[username] = account;
}

void Accounts::LoadAccounts(std::string filename) {
    std::shared_ptr<Node> account_node = SkexmlParser::Parse(filename);
    auto children = account_node->GetChildren();
    for(auto child : children)
    {
        std::shared_ptr<Node> child_node = child.second;
        std::string username = child_node->GetAttribute("Username");
        std::string password_string = child_node->GetAttribute("Password");
        std::string account_level = child_node->GetAttribute("AccountLevel");
        size_t password = (size_t)atol(password_string.c_str());
        AddAccount(username, password, std::atoi(account_level.c_str()));
    }
}

void Accounts::SaveAccounts(std::string filename) {
    std::string node_name = "Accounts";
    std::shared_ptr<Node> parent = std::make_shared<Node>(node_name);
    Account account;
    for(auto account_tuple: _accounts)
    {
        account = account_tuple.second;
        std::string username = account.GetUsername();
        std::string password = std::to_string(account.GetPassword());
        std::string account_level = std::to_string(account.GetAccountLevel());
        std::shared_ptr<Node> child = std::make_shared<Node>(username);
        child->AddAttribute("Username", username);
        child->AddAttribute("Password", password);
        child->AddAttribute("AccountLevel", account_level);
        parent->AddChild(child);
    }
    SkexmlParser::BuildSkeXML(filename, parent);
}

Account Accounts::GetAccount(std::string username) {
    auto account = _accounts.find(username);
    if(account != _accounts.end())
        return account->second;
    return Account();
}

bool Accounts::IsAccount(std::string username) {
    return _accounts.find(username) != _accounts.end();
}

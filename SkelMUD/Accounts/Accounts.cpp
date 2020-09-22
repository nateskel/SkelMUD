//
// Created by skelton on 3/17/16.
//

#include "Accounts.h"
#include "../Skexml/SkexmlParser.h"
#include "../GameData.h"

void Accounts::AddAccount(Account account) {
    m_accounts[account.GetUsername()] = account;
}

void Accounts::AddAccount(std::string username, std::string password, int account_level) {
    Account account(username, password, account_level);
    m_accounts[username] = account;
}

void Accounts::AddAccount(std::string username, size_t password, int account_level) {
    Account account(username, password, account_level);
    m_accounts[username] = account;
}

void Accounts::LoadAccounts(std::string filename) {
    std::shared_ptr<Node> account_node = SkexmlParser::Parse(filename);
    if(account_node->GetName() == "")
        return;
    auto children = account_node->GetChildren();
    for(auto child : children)
    {
        std::shared_ptr<Node> child_node = child.second;
        std::string username = child_node->GetAttribute("Username");
        std::string password_string = child_node->GetAttribute("Password");
        std::string account_level = child_node->GetAttribute("AccountLevel");
        auto characters = child_node->GetListAttribute("Characters");
        size_t password = (size_t)atol(password_string.c_str());
        Account account(username, password, std::atoi(account_level.c_str()));
        for(auto char_string: characters)
        {
            account.AddCharacter(char_string);
        }
        AddAccount(account);
    }
}

void Accounts::SaveAccounts(std::string filename) {
    std::string node_name = "Accounts";
    std::shared_ptr<Node> parent = std::make_shared<Node>(node_name);
    Account account;
    for(auto account_tuple: m_accounts)
    {
        account = account_tuple.second;
        std::string username = account.GetUsername();
        std::string password = std::to_string(account.GetPassword());
        std::string account_level = std::to_string(account.GetAccountLevel());
        std::shared_ptr<Node> child = std::make_shared<Node>(username);
        child->AddAttribute("Username", username);
        child->AddAttribute("Password", password);
        child->AddAttribute("AccountLevel", account_level);
        std::stringstream ss;
        auto characters = account.GetCharacters();
        child->AddList("Characters", characters);
        parent->AddChild(child);
        ss << GameData::ACCOUNT_DATA << account.GetUsername();
        Utils::make_directory(ss.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    SkexmlParser::BuildSkeXML(filename, parent);
    std::stringstream ss;
}

Account Accounts::GetAccount(std::string username) {
    auto account = m_accounts.find(username);
    if(account != m_accounts.end())
        return account->second;
    return Account();
}

bool Accounts::IsAccount(std::string username) {
    return m_accounts.find(username) != m_accounts.end();
}

void Accounts::AddCharacter(std::string username, std::string character_name) {
    Account account = GetAccount(username);
    account.AddCharacter(character_name);
    AddAccount(account);
}
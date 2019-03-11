//
// Created by skelton on 3/17/16.
//

#include <sstream>
#include "LoginState.h"
#include "../Sender.h"
#include "StateFactory.h"
#include "../GameData.h"

void LoginState::processInput(const std::string& input, std::shared_ptr<Connection> connection) {
    int state = m_state_map[connection->GetID()];
    switch(state) {
        case USERNAME:
            processUsername(input, connection);
            break;
        case PASSWORD:
            processPassword(input, connection);
            break;
        case NEWPASSWORD:
            processNewPassword(input, connection);
            break;
        case NEWACCOUNT:
            processNewAccount(input, connection);
            break;
        default:
            break;
    }
}

void LoginState::RemoveConnection(const int &connection_id) {
    m_state_map.erase(connection_id);
}

void LoginState::reset(std::shared_ptr<Connection> connection) {
    connection->SetUsername("");
    connection->GetAccount().SetUsername("");
    init(connection);
    m_state_map[connection->GetID()] = USERNAME;
}

void LoginState::init(std::shared_ptr<Connection> connection) {
    GameState::init(connection);
    Sender::Send("Welcome to SkelMUD!\r\nPlease enter your username\r\n", connection);
    connection->SetPrompt(GetPrompt(*connection));
    int connection_id = connection->GetID();
    if(m_state_map.find(connection_id) == m_state_map.end())
        m_state_map[connection_id] = USERNAME;
}


void LoginState::processPassword(const std::string &input, std::shared_ptr<Connection> connection) {
    Account account = connection->GetAccount();
    int connection_id = connection->GetID();
    if (account.MatchPassword(input)) {
        Sender::Send("Logged In!\r\n", connection);
        connection->SetState(GameStates::CHARACTERCREATION, game_data);
        m_state_map.erase(connection_id);
    }
    else {
        Sender::Send("Bad password\r\n", connection);
        connection->SetUsername("");
        account.SetUsername("");
        // TODO: change to allow for 3 attempts
        reset(connection);
    }
}

void LoginState::processNewPassword(const std::string &input, std::shared_ptr<Connection> connection) {
    Account account = connection->GetAccount();
    int connection_id = connection->GetID();
    //TODO: validate password
    account.SetUsername(connection->GetUsername());
    account.SetPassword(input);
    game_data->AddAccount(account);
    game_data->SaveAccounts(GameData::ACCOUNT_FILE);
    Sender::Send("Account created\r\n", connection);
    connection->SetState(GameStates::CHARACTERCREATION, game_data);
    m_state_map.erase(connection_id);
}

void LoginState::processNewAccount(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "y" or input == "Y") {
        Sender::Send("Enter password for the account\r\n", connection);
        m_state_map[connection->GetID()] = NEWPASSWORD;
    }
    else
        reset(connection);
}

void LoginState::processUsername(const std::string &input, std::shared_ptr<Connection> connection) {
    connection->SetUsername(input);
    auto accounts = game_data->GetAccounts();
    if(accounts.IsAccount(input))
    {
        Sender::Send("Password: \r\n", connection);
        connection->SetAccount(accounts.GetAccount(input));
        m_state_map[connection->GetID()] = PASSWORD;
    }
    else
    {
        Sender::Send("No account found, would you like to make a new one? Y/N\r\n", connection);
        m_state_map[connection->GetID()] = NEWACCOUNT;
    }
}

void LoginState::Shutdown(std::shared_ptr<Connection> connection) {
}

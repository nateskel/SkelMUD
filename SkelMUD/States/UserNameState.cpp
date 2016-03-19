//
// Created by skelton on 3/17/16.
//

#include <sstream>
#include "UserNameState.h"
#include "../Sender.h"

void UserNameState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    std::string username = connection->GetUsername();
    Account account = connection->GetAccount();
    if(username == "")
    {
        connection->SetUsername(input);
        processUsername(connection);
    }
    else if(account.GetUsername() == username)
    {
        if(account.MatchPassword(input))
        {
            Sender::Send("Logged In!\r\n", connection);
            connection->SetState("Playing");
        }
        else
        {
            Sender::Send("Bad password\r\n", connection);
            connection->SetUsername("");
            account.SetUsername("");
            // TODO: change to allow for 3 attempts
            init(connection);
        }
    }
    else if(connection->GetPassword() == "-1")
    {
        //TODO: validate password
        account.SetUsername(username);
        account.SetPassword(input);
        //auto accounts = game_data->GetAccounts();
        game_data->AddAccount(account);
        game_data->SaveAccounts("/home/skelton/SkelMUD/SkelMUD/SkelMUD/Accounts.sml");
        Sender::Send("Account created\r\n", connection);
        connection->SetState("Playing");
    }
    else if(input == "y" or input == "Y")
    {
        Sender::Send("Enter password for the account\r\n", connection);
        connection->SetPassword("-1");
    }
    else
    {
        connection->SetUsername("");
        account.SetUsername("");
        init(connection);
    }
}

void UserNameState::init(std::shared_ptr<Connection> connection) {
    GameState::init(connection);
    Sender::Send("Welcome to SkelMUD!\r\nPlease enter your username\r\n", connection);
    connection->SetPrompt(GetPrompt(connection));
}

void UserNameState::processUsername(std::shared_ptr<Connection> connection) {
    auto accounts = game_data->GetAccounts();
    if(accounts.IsAccount(connection->GetUsername()))
    {
        Sender::Send("Password: \r\n", connection);
        connection->SetAccount(accounts.GetAccount(connection->GetUsername()));
    }
    else
    {
        Sender::Send("No account found, would you like to make a new one? Y/N\r\n", connection);
    }
}

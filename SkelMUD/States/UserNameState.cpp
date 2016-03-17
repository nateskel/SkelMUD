//
// Created by skelton on 3/17/16.
//

#include <sstream>
#include "UserNameState.h"
#include "../Sender.h"

void UserNameState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    ss << "Welcome " << input << "\r\n";
    Accounts accounts = game_data->GetAccounts();
    if(accounts.IsAccount(input))
        ss << "I have found your account!";
    else
        ss << "You do not have an account";
    Sender::Send(ss.str(), connection);
    connection->SetState("Playing");
}

void UserNameState::init(std::shared_ptr<Connection> connection) {
    GameState::init(connection);
    Sender::Send("Welcome to SkelMUD!\r\nPlease enter your username", connection);
}

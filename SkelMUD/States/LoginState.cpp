//
// Created by nate on 11/18/15.
//

#include "LoginState.h"
#include "../Sender.h"

void LoginState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    Sender::Send("Welcome, \r\n", connection);
    Sender::Send(input, connection);
    connection->SetState("Playing");
}

void LoginState::init(std::shared_ptr<Connection> connection) {
//    connection->AddOutput("Welcome to the chat!\r\nEnter Username: ");
    Sender::Send("Welcome to the chat!\r\nEnter Username: \r\n", connection);
}

std::string LoginState::GetPrompt(std::shared_ptr<Connection> connection) {
    return connection->GetPrompt();
}

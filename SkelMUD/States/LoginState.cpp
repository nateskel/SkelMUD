//
// Created by nate on 11/18/15.
//

#include "LoginState.h"

void LoginState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    connection->AddOutput("Welcome, ");
    connection->AddOutput(input);
}

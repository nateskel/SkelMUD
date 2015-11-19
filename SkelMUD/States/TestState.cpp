//
// Created by nate on 11/18/15.
//

#include "TestState.h"

void TestState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    connection->AddOutput("ASDF");
}

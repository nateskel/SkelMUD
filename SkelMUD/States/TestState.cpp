//
// Created by nate on 11/18/15.
//

#include "TestState.h"

void TestState::processInput(const std::string &input, std::shared_ptr<Connection> connection) {
    connection->AddOutput("ASDF");
}

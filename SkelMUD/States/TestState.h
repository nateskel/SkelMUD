//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_TESTSTATE_H
#define SKELMUD_TESTSTATE_H


#include "GameState.h"

class TestState : public GameState{
public:
    virtual void processInput(const std::string &input, std::shared_ptr<Connection> connection) override;
    TestState(std::shared_ptr<GameData> data) : GameState(data) { };
};


#endif //SKELMUD_TESTSTATE_H

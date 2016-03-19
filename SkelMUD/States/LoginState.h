//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_LOGINSTATE_H
#define SKELMUD_LOGINSTATE_H


#include "GameState.h"

class LoginState : public GameState {
public:
    void processInput(std::string input, std::shared_ptr<Connection> connection);

    LoginState(std::shared_ptr<GameData> data) : GameState(data) {  }
private:

public:
    virtual void init(std::shared_ptr<Connection> connection) override;

    virtual std::string GetPrompt(std::shared_ptr<Connection> connection) override;
};


#endif //SKELMUD_LOGINSTATE_H

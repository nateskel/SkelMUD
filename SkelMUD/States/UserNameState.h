//
// Created by skelton on 3/17/16.
//

#ifndef SKELMUD_USERNAMESTATE_H
#define SKELMUD_USERNAMESTATE_H


#include "GameState.h"

class UserNameState : public GameState {

private:
    void processUsername(std::shared_ptr<Connection> connection);
public:
    UserNameState(std::shared_ptr<GameData> data) : GameState(data) { }
    virtual void processInput(std::string input, std::shared_ptr<Connection> connection) override;
    virtual void init(std::shared_ptr<Connection> connection) override;
};


#endif //SKELMUD_USERNAMESTATE_H

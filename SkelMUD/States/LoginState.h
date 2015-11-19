//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_LOGINSTATE_H
#define SKELMUD_LOGINSTATE_H


#include "GameState.h"

class LoginState : public GameState {
public:
    void processInput(std::string input, std::shared_ptr<Connection> connection);

    LoginState(GameData data) : GameState(data) {  }
private:

};


#endif //SKELMUD_LOGINSTATE_H

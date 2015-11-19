//
// Created by nate on 11/17/15.
//

#ifndef SKELMUD_GAMESTATE_H
#define SKELMUD_GAMESTATE_H


#include <map>
#include "../Connection.h"
#include "../GameData.h"

class GameState {
public:
    GameState() {};
    GameState(GameData& data) : game_data(data) {};
    virtual void processInput(std::string input, std::shared_ptr<Connection> connection);
protected:
    GameData game_data;
};

#endif //SKELMUD_GAMESTATE_H

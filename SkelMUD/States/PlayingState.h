//
// Created by nate on 11/22/15.
//

#ifndef SKELMUD_PLAYINGSTATE_H
#define SKELMUD_PLAYINGSTATE_H


#include "GameState.h"

class PlayingState : public GameState {
public:
    virtual void processInput(std::string input, std::shared_ptr<Connection> connection) override;

    PlayingState(std::shared_ptr<GameData> data) : GameState(data) {  }

    virtual void init(std::shared_ptr<Connection> connection) override;

    virtual std::string GetPrompt(std::shared_ptr<Connection> connection) override;
};


#endif //SKELMUD_PLAYINGSTATE_H

//
// Created by nate on 7/5/16.
//

#ifndef SKELMUD_CREATECHARACTERSTATE_H
#define SKELMUD_CREATECHARACTERSTATE_H


#include "GameState.h"

class CreateCharacterState : public GameState {
private:
    enum char_creation_states {
        SELECT_CHARACTER,
        CHOOSE_RACE,
        CHOOSE_CLASS,
        ROLL_STATS
    };
    std::map<int, int> m_state_map;
    void processSelectCharacter(const std::string &input, std::shared_ptr<Connection> connection);
    void processChooseRace(const std::string &input, std::shared_ptr<Connection> connection);
    void processChooseClass(const std::string &input, std::shared_ptr<Connection> connection);
    void processRollStats(const std::string &input, std::shared_ptr<Connection> connection);
    std::string rollStats();

public:
    CreateCharacterState(std::shared_ptr<GameData> data) : GameState(data) { }
    
    virtual void processInput(const std::string& input, std::shared_ptr<Connection> connection) override;

    virtual void init(std::shared_ptr<Connection> connection) override;

    virtual std::string GetPrompt(std::shared_ptr<Connection> connection) override;
};


#endif //SKELMUD_CREATECHARACTERSTATE_H

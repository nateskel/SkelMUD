//
// Created by nate on 7/5/16.
//

#ifndef SKELMUD_CREATECHARACTERSTATE_H
#define SKELMUD_CREATECHARACTERSTATE_H


#include "GameState.h"
#include "../Races/Race.h"
#include "../Classes/CharacterClass.h"
#include "../GameData.h"

class CreateCharacterState : public GameState {
private:
    enum char_creation_states {
        SELECT_CHARACTER,
        CHOOSE_RACE,
        CHOOSE_CLASS,
        ROLL_STATS,
        NAME_CHARACTER,
        CONFIRM_CHARACTER
    };
    std::map<int, int> m_state_map;
    std::map<int, Race> m_race_map;
    std::map<int, CharacterClass> m_class_map;
    void processSelectCharacter(const std::string &input, std::shared_ptr<Connection> connection);
    void processChooseRace(const std::string &input, std::shared_ptr<Connection> connection);
    void processChooseClass(const std::string &input, std::shared_ptr<Connection> connection);
    void processNameCharacter(const std::string &input, std::shared_ptr<Connection> connection);
    void processRollStats(const std::string &input, std::shared_ptr<Connection> connection);
    void processConfirmCharacter(const std::string &input, std::shared_ptr<Connection> connection);
    std::string rollStats();

public:
    CreateCharacterState(std::shared_ptr<GameData> data) : GameState(data) {
        m_race_map = data->GetRaces().EnumerateRaces();
        m_class_map = data->GetClasses().EnumerateClasses();
    }
    
    virtual void processInput(const std::string& input, std::shared_ptr<Connection> connection) override;

    virtual void init(std::shared_ptr<Connection> connection) override;

    virtual std::string GetPrompt(std::shared_ptr<Connection> connection) override;

    virtual void Shutdown(std::shared_ptr<Connection> connection) override;
};


#endif //SKELMUD_CREATECHARACTERSTATE_H

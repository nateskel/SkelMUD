//
// Created by skelton on 8/26/15.
//

#ifndef SKELMUD_CHARACTERCREATION_H
#define SKELMUD_CHARACTERCREATION_H

#include <vector>
#include <map>
#include "CharacterClass.h"
#include "Player.h"

class CharacterCreator {
public:

    CharacterCreator(const std::vector<CharacterClass> &classes);

    CharacterCreator() { }

    enum CreatorState {
        SEX, RACE, CLASS, ATTRIBUTES, NAME, CONFIRM
    };

    std::string processNext(std::string data);
    Player* getPlayer();
    std::string showClassList();
    std::string processClassSelection(std::string data);
    std::string showNameMessage();
    std::string processNameSelection(std::string data);
    std::string signalComplete();

private:
    CreatorState m_state;
    Player* m_player;
    std::map<CharacterCreator::CreatorState, std::string (CharacterCreator::*)(std::string)> m_state_map;
    std::vector<CharacterClass> classes;

    void initializeStates();
};

#endif //SKELMUD_CHARACTERCREATION_H

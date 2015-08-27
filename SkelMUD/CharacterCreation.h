//
// Created by skelton on 8/26/15.
//

#ifndef SKELMUD_CHARACTERCREATION_H
#define SKELMUD_CHARACTERCREATION_H

#include <vector>
#include "CharacterClass.h"
#include "Player.h"

class CharacterCreator {
public:

    CharacterCreator(const std::vector<CharacterClass> &classes) : classes(classes) { }

    CharacterCreator() { }

    enum CreatorState {
        SEX, RACE, CLASS, ATTRIBUTES, NAME, CONFIRM
    };

    std::string processNext(std::string data);

    Player* getPlayer();

    std::string showClassList();

    void processClassSelection(std::string data);

    std::string showNameMessage();

    void processNameSelection();

    std::string signalComplete();

private:
    CreatorState m_state;

    Player* m_player;

    std::vector<CharacterClass> classes;
};

#endif //SKELMUD_CHARACTERCREATION_H

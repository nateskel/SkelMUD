//
// Created by nate on 11/11/17.
//

#ifndef SKELMUD_STATEFACTORY_H
#define SKELMUD_STATEFACTORY_H


#include <string>
#include <memory>
#include <map>

class GameState;

class GameData;

enum GameStates : short {
    LOGIN,
    PLAYING,
    CHARACTERCREATION,
    BUILDING
};

class StateFactory {
public:
    static std::unique_ptr<GameState> GetGameState(GameStates state, std::shared_ptr<GameData> game_data);
};


#endif //SKELMUD_STATEFACTORY_H

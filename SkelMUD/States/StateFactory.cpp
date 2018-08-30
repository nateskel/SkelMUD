//
// Created by nate on 11/11/17.
//

#include "StateFactory.h"
#include "GameState.h"
#include "../GameData.h"
#include "LoginState.h"
#include "PlayingState.h"
#include "CreateCharacterState.h"
#include "BuildingState.h"
#include "TestState.h"

std::unique_ptr<GameState> StateFactory::GetGameState(GameStates state, std::shared_ptr<GameData> game_data) {
    switch(state) {
        case LOGIN:
            return std::unique_ptr<LoginState>(new LoginState(game_data));
        case PLAYING:
            return std::unique_ptr<PlayingState>(new PlayingState(game_data));
        case CHARACTERCREATION:
            return std::unique_ptr<CreateCharacterState>(new CreateCharacterState(game_data));
        case BUILDING:
            return std::unique_ptr<BuildingState>(new BuildingState(game_data));
        default:
            return std::unique_ptr<TestState>(new TestState(game_data));
    }
}

//
// Created by nate on 11/17/15.
//

#include "GameState.h"
#include "../Format.h"

//void GameState::processInput(std::string input, std::shared_ptr<Connection> connection) {
//
//}

void GameState::init(std::shared_ptr<Connection> connection) {

}

GameState::GameState(std::shared_ptr<GameData> data) {
    game_data = data;
}

std::string GameState::GetPrompt(std::shared_ptr<Connection> connection) {
    return Format::YELLOW + "> ";
}

//
// Created by nate on 11/17/15.
//

#include "GameState.h"
#include "../Format.h"
#include "../GameData.h"

//void GameState::processInput(std::string input, std::shared_ptr<Connection> connection) {
//
//}

void GameState::init(std::shared_ptr<Connection> connection) {

}

GameState::GameState(std::shared_ptr<GameData> data) {
    game_data = data;
}

std::string GameState::GetPrompt(Connection connection) {
    return Format::YELLOW + "> \r\n";
}

std::string GameState::GetLastPrompt() {
    return m_prev_prompt;
}

bool GameState::IsDirty(std::string prompt) {
    return prompt != m_prev_prompt;
}

void GameState::CleanPrompt(std::string prompt) {
    m_prev_prompt = prompt;
}

bool GameState::IsDirty(Connection connection) {
    return GetPrompt(connection) != m_prev_prompt;
}

void GameState::CleanPrompt(Connection connection) {
    m_prev_prompt = GetPrompt(connection);
}

//
// Created by nate on 11/22/15.
//

#include "PlayingState.h"
#include "../Tokenizer.h"
#include "../Sender.h"

void PlayingState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    //std::string command = Tokenizer::GetFirstToken(input);
    Sender::SendAll(connection->GetUsername() + ": " + input, game_data->GetAllConnections(), connection->GetSocket());
}

void PlayingState::init(std::shared_ptr<Connection> connection) {
    Sender::Send("Welcome to the chat, " + connection->GetUsername(), connection);
}

//
// Created by nate on 11/22/15.
//

#include "PlayingState.h"
#include "../Tokenizer.h"
#include "../Sender.h"
#include "../Format.h"

void PlayingState::processInput(std::string input, std::shared_ptr<Connection> connection) {
    //std::string command = Tokenizer::GetFirstToken(input);

    Sender::SendAll("\r\n" + connection->GetUsername() + ": " + input + "\r\n", game_data->GetLoggedInConnections(), connection->GetSocket());
}

void PlayingState::init(std::shared_ptr<Connection> connection) {
    Sender::Send("Welcome to the chat, " + connection->GetUsername() + "\r\n", connection);
    connection->SetPrompt(GetPrompt(connection));
    connection->SetLoggedIn(true);
}

std::string PlayingState::GetPrompt(std::shared_ptr<Connection> connection) {
    return Format::YELLOW + "<" + Format::BLUE + connection->GetUsername() + Format::YELLOW + "> ";
}

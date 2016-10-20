//
// Created by nate on 11/22/15.
//

#include <sstream>
#include "PlayingState.h"
#include "../Tokenizer.h"
#include "../Sender.h"
#include "../Format.h"

void PlayingState::processInput(const std::string& input, std::shared_ptr<Connection> connection) {
    //std::string command = Tokenizer::GetFirstToken(input);
    if(input == "quit")
//        Sender::Send("Goodbye", connection);
        connection->Close();
    Sender::SendAll("\r\n" + connection->GetUsername() + ": " + input + "\r\n", game_data->GetLoggedInConnections(), connection->GetSocket());
}

void PlayingState::init(std::shared_ptr<Connection> connection) {
    Sender::Send("Welcome to the chat, " + connection->GetCharacterName() + "\r\n", connection);
    connection->SetPrompt(GetPrompt(connection));
    connection->SetLoggedIn(true);
}

std::string PlayingState::GetPrompt(std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    ss << Format::YELLOW << "<" + Format::BLUE << connection->GetCharacterName() << Format::YELLOW << "> ";
    ss << Format::RED << "<" << connection->GetHealth() << "> \r\n";
    return ss.str();
}

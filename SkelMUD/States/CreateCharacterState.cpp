//
// Created by nate on 7/5/16.
//

#include <sstream>
#include "CreateCharacterState.h"
#include "../Sender.h"

void CreateCharacterState::processInput(const std::string& input, std::shared_ptr<Connection> connection) {
    std::string username = connection->GetUsername();
    Account account = connection->GetAccount();
    int state = m_state_map[connection->GetID()];
    switch(state)
    {
        case SELECT_CHARACTER:
            processSelectCharacter(input, connection);
            break;
        case CHOOSE_RACE:
            processChooseRace(input, connection);
            break;
        case CHOOSE_CLASS:
            processChooseClass(input, connection);
            break;
        case ROLL_STATS:
            processRollStats(input, connection);
            break;
        default:
            break;

    }
}

void CreateCharacterState::init(std::shared_ptr<Connection> connection) {
//    Sender::Send("Placeholder Character Select/Creation\r\nEnter anything to continue\r\n", connection);
    std::stringstream ss;
    ss << connection->GetAccount().GetCharacterList() << "\r\n";
    ss << "Choose a character or enter <N> to create a new character\r\n";
    Sender::Send(ss.str(), connection);
    int connection_id = connection->GetID();
    if(m_state_map.find(connection_id) == m_state_map.end())
        m_state_map[connection_id] = SELECT_CHARACTER;
}

std::string CreateCharacterState::GetPrompt(std::shared_ptr<Connection> connection) {
    return GameState::GetPrompt(connection);
}

void CreateCharacterState::processSelectCharacter(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "N" or input == "n") {
        std::stringstream ss;
        int count = 0;
        auto races = game_data->GetRaces().GetRaces();
        for(auto race : races) {
            ss << ++count << " " << race.first << "\r\n";
        }
        ss << "Choose a race\r\n";
        Sender::Send(ss.str(), connection);
        m_state_map[connection->GetID()] = CHOOSE_RACE;
    }
    else {
        // validate entry
        Sender::Send("Character Selected\r\n", connection);
    }
}

void CreateCharacterState::processChooseRace(const std::string &input, std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    int count = 0;
    auto char_classes = game_data->GetClasses().GetClasses();
    for(auto char_class : char_classes) {
        ss << ++count << " " << char_class.first << "\r\n";
    }
    ss << "Race Selected\r\nChoose a class\r\n";
    Sender::Send(ss.str(), connection);
    m_state_map[connection->GetID()] = CHOOSE_CLASS;
}

void CreateCharacterState::processChooseClass(const std::string &input, std::shared_ptr<Connection> connection) {
    Sender::Send("Class Selected\r\n", connection);
    //m_state_map[connection->GetID()] = ROLL_STATS;
    connection->SetState("Playing");
}

void CreateCharacterState::processRollStats(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "R" or input == "r") {
        Sender::Send("Stats Rolled, enter <R> to <R>eroll, or <A> to <A>ccept\r\n", connection);
    }
    else {
        connection->SetState("Playing");
    }
}


std::string CreateCharacterState::rollStats() {
    return "Some stats";
}



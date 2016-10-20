//
// Created by nate on 7/5/16.
//

#include <sstream>
#include "CreateCharacterState.h"
#include "../Sender.h"
#include "../Utils.h"

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
        case NAME_CHARACTER:
            processNameCharacter(input, connection);
            break;
        case CONFIRM_CHARACTER:
            processConfirmCharacter(input, connection);
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
    //if(m_state_map.find(connection_id) == m_state_map.end())
    m_state_map[connection_id] = SELECT_CHARACTER;
}

std::string CreateCharacterState::GetPrompt(std::shared_ptr<Connection> connection) {
    return GameState::GetPrompt(connection);
}

void CreateCharacterState::processSelectCharacter(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "N" or input == "n") {
        std::stringstream ss;
        for(auto race : m_race_map)
        {
            ss << race.first << " " << race.second.getRace_name() << "\r\n";
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
    int choice = 0;

    bool valid = Utils::IsNumber(input);
    if(valid) {
        choice = std::stoi(input);
        if (choice < 1 or choice > m_race_map.size())
            valid = false;
    }
    if(valid)
    {
        connection->SetCharacterRace(m_race_map[choice].getRace_name());
        std::stringstream ss;
        for(auto char_class : m_class_map)
        {
            ss << char_class.first << " " << char_class.second.GetName() << "\r\n";
        }
        ss << "Race Selected\r\nChoose a class\r\n";
        Sender::Send(ss.str(), connection);
        m_state_map[connection->GetID()] = CHOOSE_CLASS;
    }
    else {
        Sender::Send("Invalid Selection\r\n", connection);
        processSelectCharacter("N", connection);
    }
}

void CreateCharacterState::processChooseClass(const std::string &input, std::shared_ptr<Connection> connection) {
    int choice = 0;
    bool valid = Utils::IsNumber(input);
    if(valid) {
        choice = std::stoi(input);
        if (choice < 1 or choice > m_class_map.size())
            valid = false;
    }
    if(valid) {
        connection->SetCharacterClass(m_class_map[choice].GetName());
        Sender::Send("Class Selected\r\nEnter name for character\r\n", connection);
        m_state_map[connection->GetID()] = NAME_CHARACTER;
//        connection->SetState("Playing");
    }
    else {
        std::stringstream ss;
        ss << "Invalid Selection\r\n";
        for (auto char_class : m_class_map) {
            ss << char_class.first << " " << char_class.second.GetName() << "\r\n";
        }
        ss << "Choose a class\r\n";
        Sender::Send(ss.str(), connection);
        m_state_map[connection->GetID()] = CHOOSE_CLASS;
    }
}

void CreateCharacterState::processNameCharacter(const std::string &input, std::shared_ptr<Connection> connection) {
    connection->SetCharacterName(input);
    std::stringstream ss;
    ss << "Name: " << connection->GetCharacterName();
    ss << "\r\nRace: " << connection->GetCharacterRace();
    ss << "\r\nClass: " << connection->GetCharacterClass();
    ss << "\r\nCreate Character?\r\n";
    Sender::Send(ss.str(), connection);
    m_state_map[connection->GetID()] = CONFIRM_CHARACTER;
}

void CreateCharacterState::processConfirmCharacter(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "Y" or input == "y" or input == "Yes" or input == "yes") {
        //connection->GetAccount().AddCharacter(connection->GetCharacterName());
//        Accounts accounts = game_data->GetAccounts();
//        Account account = accounts.GetAccount(connection->GetUsername());
//        account.AddCharacter(connection->GetCharacterName());
        game_data->AddCharacter(connection->GetUsername(), connection->GetCharacterName());

        game_data->SaveAccounts(GameData::ACCOUNT_FILE);
        connection->SetState("Playing");
    }
    else
        init(connection);
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



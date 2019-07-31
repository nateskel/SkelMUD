//
// Created by nate on 7/5/16.
//

#include <sstream>
#include "CreateCharacterState.h"
#include "../Sender.h"
#include "../Utils.h"
#include "StateFactory.h"
#include "../Tokenizer.h"

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
    CleanPrompt(*connection);
    std::stringstream ss;
    ss << connection->GetAccount().GetCharacterList() << "\r\n";
    ss << "Choose a character or enter <N> to create a new character\r\n";
    Sender::Send(ss.str(), connection);
    int connection_id = connection->GetID();
    //if(m_state_map.find(connection_id) == m_state_map.end())
    m_state_map[connection_id] = SELECT_CHARACTER;
}

std::string CreateCharacterState::GetPrompt(Connection connection) {
    return GameState::GetPrompt(connection);
}

void CreateCharacterState::processSelectCharacter(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "N" or input == "n") {
        std::stringstream ss;
        for(auto race : m_race_map)
        {
            ss << race.first << " " << race.second->getRace_name() << "\r\n";
        }
        ss << "Choose a race\r\n";
        Sender::Send(ss.str(), connection);
        m_state_map[connection->GetID()] = CHOOSE_RACE;
    }
    else {
        // validate entry
        auto characters = game_data->GetAccount(connection->GetUsername()).GetCharacters();
        int selection = 0;
        if(Utils::IsNumber(input))
            selection = std::stoi(input.c_str());
        if(selection < 1 or selection > characters.size())
        {
            Sender::Send("Invalid Selection\n", connection);
        }
        else {
            std::string character = characters[selection - 1];
            std::shared_ptr<Player> player = game_data->GetPlayer(character);
            player->SetID(connection->GetID());
            connection->SetPlayer(player);
            connection->SetCharacterName(player->GetPlayerName());
            connection->SetCharacterClass(player->GetPlayerClass());
            connection->SetCharacterRace(player->GetPlayerRaceStr());
            Sender::Send("Character Selected\r\n", connection);
            connection->SetState(GameStates::PLAYING, game_data);
        }
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
        connection->SetCharacterRace(m_race_map[choice]->getRace_name());
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
        //Sender::Send("Class Selected\r\nEnter name for character\r\n", connection);
        std::stringstream ss;
        ss << "Class Selected\r\n" << ShowStats() << "\r\n" <<  "Choose a stat (1-5), <R>eset, or <A>ccept\r\n";
        Sender::Send(ss.str(), connection);
        m_state_map[connection->GetID()] = ROLL_STATS;
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
    std::string command = input;
    Tokenizer::LowerCase(input);
    if(command == "y" or command == "yes") {
        auto character_race = game_data->GetRace(connection->GetCharacterRace());
        std::shared_ptr<Player> player = std::make_shared<Player>(connection->GetID(), connection->GetCharacterName(),
                                                                  connection->GetCharacterClass(), character_race->getRace_name());
        player->SetStrength(m_str);
        player->SetEndurance(m_end);
        player->SetIntelligence(m_int);
        player->SetDexterity(m_dex);
        player->SetSkill(m_skill);
        player->SetAttributePoints(m_att_points);
        player->SetPlayerRace(game_data->GetRace(player->GetPlayerRaceStr()));
        player->SetCredits(50000);
        game_data->AddCharacter(connection->GetUsername(), player);
        game_data->SaveCharacters(GameData::CHARACTER_FILE);
        game_data->SaveAccounts(GameData::ACCOUNT_FILE);
        connection->SetPlayer(player);
        connection->SetState(GameStates::PLAYING, game_data);
    }
    else
        init(connection);
}

void CreateCharacterState::processRollStats(const std::string &input, std::shared_ptr<Connection> connection) {
    if(input == "R" or input == "r") {
        m_str = 1;
        m_skill = 1;
        m_dex = 1;
        m_int = 1;
        m_end = 1;
        m_att_points = INIT_ATTRIBUTE_POINTS;
        Sender::Send(ShowStats(), connection);
    }
    if(input == "1" || input == "2" || input == "3" || input == "4" || input == "5") {
        if (m_att_points == 0) {
            Sender::Send("No more points to distribute\r\n", connection);
            Sender::Send(ShowStats(), connection);
            Sender::Send("<R>eset, or <A>ccept\r\n", connection);
        } else {
        if (input == "1") {
                m_str++;
            } else if (input == "2") {
                m_end++;
            } else if (input == "3") {
                m_int++;
            } else if (input == "4") {
                m_dex++;
            } else if (input == "5") {
                m_skill++;
            }
            m_att_points--;
            Sender::Send(ShowStats(), connection);
            Sender::Send("Choose a stat (1-5), <R>eset, or <A>ccept\r\n", connection);
        }
    }
    else if(input == "A"){
        //connection->SetState(GameStates::PLAYING, game_data);
        Sender::Send("Attributes confirmed\r\nEnter name for character\r\n", connection);
        m_state_map[connection->GetID()] = NAME_CHARACTER;
    }
    else {
        Sender::Send("Choose a stat (1-5), <R>eset, or <A>ccept\r\n", connection);
    }
}


std::string CreateCharacterState::ShowStats() {
    std::stringstream ss;
    ss << "1 Strength      : " << m_str << "\r\n";
    ss << "2 Endurance     : " << m_end << "\r\n";
    ss << "3 Intelligence  : " << m_int << "\r\n";
    ss << "4 Dexterity     : " << m_dex << "\r\n";
    ss << "5 Skill         : " << m_skill << "\r\n";
    ss << "Points remaining: " << m_att_points << "\r\n";
    return ss.str();
}


void CreateCharacterState::Shutdown(std::shared_ptr<Connection> connection) {

}

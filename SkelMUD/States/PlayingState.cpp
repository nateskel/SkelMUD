//
// Created by nate on 11/22/15.
//

#include <sstream>
#include "PlayingState.h"
#include "../Tokenizer.h"
#include "../Sender.h"
#include "../Format.h"
#include "../Logger.h"
#include "../Utils.h"

void PlayingState::processInput(const std::string& input, std::shared_ptr<Connection> connection) {
    std::string input_string = input;
    std::string command = Tokenizer::GetFirstToken(input_string);
    command = Tokenizer::LowerCase(command);
    if(input == "quit")
        connection->Close();
    if(m_cmd_map.find(command) != m_cmd_map.end())
        m_cmd_map[command](input_string, connection, game_data);
    else
        Sender::Send("Unrecognized command!\n", connection);
}

void PlayingState::init(std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    ss << "Welcome to SkelMUD, " << connection->GetCharacterName() << "!" << Format::NL;
    Sender::Send(ss.str(), connection);
    connection->SetPrompt(GetPrompt(connection));
    connection->SetLoggedIn(true);
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    player->SetID(connection->GetID());
    game_data->GetPlanet(0)->GetRoom(0)->AddPlayer(player);
    //TODO: set proper planet id
    player->SetPlanetID(0);
    player->SetRoomID(0);
//    player->SetVisible(true);
    CmdLook("", connection, game_data);
    ss.str(std::string());
    ss.clear();
    ss << connection->GetCharacterName() << " has logged in!\n";
    Sender::SendAll(ss.str(), game_data->GetLoggedInConnections(), connection->GetSocket());
}

std::string PlayingState::GetPrompt(std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    ss << Format::YELLOW << "<" + Format::BLUE << connection->GetCharacterName() << Format::YELLOW << "> ";
    ss << Format::RED << "<" << connection->GetHealth() << "> \n";
    return ss.str();
}

void PlayingState::CmdHelp(const std::string& input, std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data)
{
    Sender::Send("'chat' to chat\r\n'tell <character>' to direct tell\r\n'online' to see who is online\n", connection);
}

void PlayingState::CmdTell(const std::string &input, std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data) {
    std::string input_string = input;
    std::string target = Tokenizer::GetFirstToken(input_string);
    auto target_connection = game_data->GetConnection(target);
    if(target_connection == nullptr) {
        Sender::Send("That person does not exist\n", connection);
    }
    else {
        std::stringstream ss;
        ss << Format::BLUE << Format::BOLD << connection->GetCharacterName() << " tells you: " <<
                Format::MAGENTA << input_string << Format::NL;
        Sender::Send(ss.str(), game_data->GetConnection(target));
    }
}

void PlayingState::CmdChat(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string input_string = input;
    std::stringstream ss;
    ss << Format::CYAN << Format::BOLD << connection->GetCharacterName() << " chats: " << Format::MAGENTA <<
            input_string << Format::NL;
    Sender::SendAll(ss.str(), game_data->GetLoggedInConnections(), connection->GetSocket());
}

void PlayingState::CmdOnline(const std::string &input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data) {
    std::stringstream ss;
    ss << Format::BOLD << Format::YELLOW << "Players Online:" << Format::NL << Format::BLUE;
    for(auto conn: game_data->GetLoggedInConnections()) {
        ss << conn.second->GetCharacterName() << Format::NL;
    }
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdLook(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    auto room = planet->GetRoom(player->GetRoomID());
    std::stringstream ss;
    ss << Format::NL << Format::BOLD << Format::YELLOW << room->GetShortDescription() << Format::RESET << Format::NL;
    ss << room->GetLongDescription() << Format::NL;
    ss << Format::BOLD <<  Format::CYAN << "Directions:" << Format::NL << GetValidDirections(*room) << Format::NL
       << Format::RESET;
    auto other_players = room->GetVisiblePlayerNames(connection->GetID());
    for(auto other_player: other_players) {
        ss << Format::BOLD << Format::BLUE << other_player << " is here." << Format::RESET << Format::NL;
    }
    if(room->IsLandable()) {
        ss << "Ships:" << Format::NL;
        for (auto ship_id: room->GetShipIDs()) {
            ss << game_data->GetShip(ship_id)->GetShipName() << Format::NL;
        }
    }
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdNorth(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    Move(connection, game_data, Direction::NORTH);
}

void PlayingState::CmdSouth(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    Move(connection, game_data, Direction::SOUTH);
}

void PlayingState::CmdEast(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    Move(connection, game_data, Direction::EAST);
}

void PlayingState::CmdWest(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    Move(connection, game_data, Direction::WEST);
}

void PlayingState::Move(std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data,
                        Direction direction) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    int room_id = player->GetRoomID();
    int player_id = player->GetID();
    bool success = false;
    std::string arrive_string = "";
    std::string depart_string = "";
    int departed_room = 0;
    switch(direction) {
        case NORTH:
            departed_room = player->GetRoomID();
            success = planet->MoveNorth(room_id, player_id);
            arrive_string = "south";
            depart_string = "north";
            break;
        case SOUTH:
            departed_room = player->GetRoomID();
            success = planet->MoveSouth(room_id, player_id);
            arrive_string = "north";
            depart_string = "south";
            break;
        case EAST:
            departed_room = player->GetRoomID();
            success = planet->MoveEast(room_id, player_id);
            arrive_string = "west";
            depart_string = "east";
            break;
        case WEST:
            departed_room = player->GetRoomID();
            success = planet->MoveWest(room_id, player_id);
            arrive_string = "east";
            depart_string = "west";
            break;
        default:
            Logger::Error("Unhandled Direction");
    }
    if(success) {
        if(game_data->GetPlayer(connection->GetCharacterName())->IsVisible()) {
            std::stringstream departed_ss;
            std::stringstream arrived_ss;
            std::string player_name = player->GetPlayerName();
            departed_ss << player_name << " has left to the " << depart_string << Format::NL;
            arrived_ss << player_name << " has arrived from the " << arrive_string << Format::NL;
            Sender::SendToMultiple(departed_ss.str(), game_data->GetLoggedInConnections(),
                                   planet->GetRoom(departed_room)->GetVisiblePlayers());
            Sender::SendToMultiple(arrived_ss.str(), game_data->GetLoggedInConnections(),
                                   planet->GetRoom(player->GetRoomID())->GetVisiblePlayers(connection->GetID()));
        }
        CmdLook("", connection, game_data);
    }
    else
        Sender::Send("Can't go that way!\n", connection);
}

void PlayingState::CmdBuild(const std::string& input, std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data) {
    if(connection->GetAccount().GetAccountLevel() > Account::NORMAL)
        connection->SetState("Building");
    else
        Sender::Send("Not Authorized to enter build mode\n", connection);
}

std::string PlayingState::GetValidDirections(Room &room) {
    std::stringstream ss;
    if(room.GetNorth() != -1)
        ss << "north ";
    if(room.GetSouth() != -1)
        ss << "south ";
    if(room.GetEast() != -1)
        ss << "east ";
    if(room.GetWest() != -1)
        ss << "west ";
    ss << "\n";
    return ss.str();
}

void PlayingState::CmdGoto(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string input_string = std::string(input);
    std::string room_number_string = Tokenizer::GetFirstToken(input_string);
    int newroom = 0;
    if(Utils::IsNumber(room_number_string)) {
        newroom = std::atoi(room_number_string.c_str());
    }

    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    if(planet->ChangeRoom(player->GetRoomID(), newroom, player->GetID())) {
        CmdLook("", connection, game_data);
    }
    else {
        Sender::Send("Room does not exist!\n", connection);
    }
}
//
// Created by nate on 10/28/16.
//

#include <sstream>
#include "BuildingState.h"
#include "../Format.h"
#include "../Sender.h"
#include "../Tokenizer.h"

void BuildingState::init(std::shared_ptr<Connection> connection) {
    Sender::Send("Build Mode\r\n", connection);
    connection->SetPrompt(GetPrompt(connection));
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    player->SetVisible(false);
    CmdLook("", connection, game_data);
}

std::string BuildingState::GetPrompt(std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    ss << Format::YELLOW << "<" + Format::GREEN << connection->GetCharacterName() << Format::YELLOW << "> " << Format::NL;
    return ss.str();
}

void BuildingState::CmdBuildRoom(const std::string &input, std::shared_ptr<Connection> connection,
                                 std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    std::shared_ptr<Room> room = std::make_shared<Room>();
    u_long room_num = planet->AddRoom(room);
    if(input.length() > 0)
    {
        std::string input_string = std::string(input);
        std::stringstream ss;
        ss << Tokenizer::GetFirstToken(input_string) << " ";
        ss << room_num << " ";
        ss << input_string;
        CmdLink(ss.str(), connection, game_data);
    }
    CmdGoto(std::to_string(room_num), connection, game_data);
}

void BuildingState::CmdLink(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    auto room = planet->GetRoom(player->GetRoomID());
    std::string input_string = std::string(input);
    std::string direction_string = Tokenizer::GetFirstToken(input_string);
    direction_string = Tokenizer::LowerCase(direction_string);
    std::string destination_id_string = Tokenizer::GetFirstToken(input_string);
    std::string both_string = Tokenizer::GetFirstToken(input_string);
    int destination_id = std::atoi(destination_id_string.c_str());
    auto destination_room = planet->GetRoom(destination_id);
    Direction direction = BuildingState::m_direction_map[direction_string];
    switch (direction) {
        case Direction::NORTH:
            room->SetNorth(destination_id);
            if(both_string == "both")
                destination_room->SetSouth(room->GetID());
            break;
        case Direction::SOUTH:
            room->SetSouth(destination_id);
            if(both_string == "both")
                destination_room->SetNorth(room->GetID());
            break;
        case Direction::EAST:
            room->SetEast(destination_id);
            if(both_string == "both")
                destination_room->SetWest(room->GetID());
            break;
        case Direction::WEST:
            room->SetWest(destination_id);
            if(both_string == "both")
                destination_room->SetEast(room->GetID());
            break;
        default:
            Sender::Send("Unable to link room\r\n", connection);
            break;
    }
}

void BuildingState::CmdGetRoomID(const std::string &input, std::shared_ptr<Connection> connection,
                                 std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    // auto planet = game_data->GetPlanet(player->GetPlanetID());
    // auto room = planet->GetRoom(player->GetRoomID());
    std::stringstream ss;
    ss << "Room ID is " << player->GetRoomID() << Format::NL;
    Sender::Send(ss.str(), connection);
}

void BuildingState::CmdSetLongDesc(const std::string &input, std::shared_ptr<Connection> connection,
                                   std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    auto room = planet->GetRoom(player->GetRoomID());
    room->SetLongDesc(input);
}

void BuildingState::CmdSetShortDesc(const std::string &input, std::shared_ptr<Connection> connection,
                                    std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    auto room = planet->GetRoom(player->GetRoomID());
    room->SetShortDesc(input);
}

void BuildingState::CmdPlay(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    connection->SetState("Playing");
}

void BuildingState::CmdSavePlanet(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto planet = game_data->GetPlanet(game_data->GetPlayer(connection->GetCharacterName())->GetPlanetID());
    game_data->SavePlanet(planet->GetID());
}

std::map<std::string, PlayingState::Direction> BuildingState::m_direction_map = {
        {"n", Direction::NORTH},
        {"north", Direction::NORTH},
        {"s", Direction::SOUTH},
        {"south", Direction::SOUTH},
        {"e", Direction::EAST},
        {"east", Direction::EAST},
        {"w", Direction::WEST},
        {"west", Direction::WEST}
};
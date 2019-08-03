//
// Created by nate on 10/28/16.
//

#include <sstream>
#include "BuildingState.h"
#include "../Format.h"
#include "../Sender.h"
#include "../Tokenizer.h"
#include "../Player.h"
#include "../Planets/Planet.h"
#include "../GameData.h"
#include "StateFactory.h"
#include <algorithm>

void BuildingState::init(std::shared_ptr<Connection> connection) {
    Sender::Send("Build Mode\r\n", connection);
    CleanPrompt(*connection);
    //connection->SetPrompt(GetPrompt(*connection));
//    auto player = connection->GetPlayer();
//    player->SetVisible(false);
    CmdLook("", connection, game_data);
}

std::string BuildingState::GetPrompt(Connection connection) {
    std::stringstream ss;
    ss << Format::YELLOW << "<" + Format::GREEN << connection.GetCharacterName() << Format::YELLOW << "> " << Format::NL;
    return ss.str();
}

void BuildingState::CmdBuildRoom(const std::string &input, std::shared_ptr<Connection> connection,
                                 std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto planet = player->GetPlanet();
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

void BuildingState::CmdDeleteRoom(const std::string &input, std::shared_ptr<Connection> connection,
                                 std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto planet = player->GetPlanet();
    auto room = player->GetRoom();
    auto rooms = planet->GetRooms();
    int room_id;
    int room_delete_id = room->GetID();

    // not allowed to delete room 0
    if(room_delete_id == 0) {
        Sender::Send("Cannot delete room 0!\n", connection);
        return;
    }
    CmdUnlink("north both", connection, game_data);
    CmdUnlink("south both", connection, game_data);
    CmdUnlink("east both", connection, game_data);
    CmdUnlink("west both", connection, game_data);
    for(auto room_to_update: rooms) {
        int room_update_id = room_to_update->GetID();
        if(room_update_id > room_delete_id)
        {
            room_to_update->SetID(room_update_id - 1);
        }
        room_id = room_to_update->GetNorth();
        if(room_id > room_delete_id) {
           room_to_update->SetNorth(room_id - 1);
        }
        room_id = room_to_update->GetSouth();
        if(room_id > room_delete_id) {
            room_to_update->SetSouth(room_id - 1);
        }
        room_id = room_to_update->GetEast();
        if(room_id > room_delete_id) {
            room_to_update->SetEast(room_id - 1);
        }
        room_id = room_to_update->GetWest();
        if(room_id > room_delete_id) {
            room_to_update->SetWest(room_id - 1);
        }
    }
    for(auto logged_in_conn: game_data->GetLoggedInConnections()) {
        CmdGoto("0", logged_in_conn.second, game_data);
    }
    planet->DeleteRoom(room_delete_id);
}

void BuildingState::CmdLink(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto planet = player->GetPlanet();
    auto room = player->GetRoom();
    std::string input_string = std::string(input);
    std::string direction_string = Tokenizer::GetFirstToken(input_string);
    direction_string = Tokenizer::LowerCase(direction_string);
    std::string destination_id_string = Tokenizer::GetFirstToken(input_string);
    std::string single_string = Tokenizer::GetFirstToken(input_string);
    int destination_id = std::atoi(destination_id_string.c_str());

    // don't process if destination doesn't exist
    if(destination_id >= planet->GetRooms().size())
        return;

    auto destination_room = planet->GetRoom(destination_id);
    Direction direction = BuildingState::m_direction_map[direction_string];
    switch (direction) {
        case Direction::NORTH:
            room->SetNorth(destination_id);
            if(single_string != "single")
                destination_room->SetSouth(room->GetID());
            break;
        case Direction::SOUTH:
            room->SetSouth(destination_id);
            if(single_string != "single")
                destination_room->SetNorth(room->GetID());
            break;
        case Direction::EAST:
            room->SetEast(destination_id);
            if(single_string != "single")
                destination_room->SetWest(room->GetID());
            break;
        case Direction::WEST:
            room->SetWest(destination_id);
            if(single_string != "single")
                destination_room->SetEast(room->GetID());
            break;
        case Direction::UP:
            room->SetUp(destination_id);
            if(single_string != "single")
                destination_room->SetDown(room->GetID());
            break;
        case Direction::DOWN:
            room->SetDown(destination_id);
            if(single_string != "single")
                destination_room->SetUp(room->GetID());
            break;
        default:
            Sender::Send("Unable to link room\r\n", connection);
            break;
    }
}

void BuildingState::CmdUnlink(const std::string& input, std::shared_ptr<Connection> connection,
                      std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto planet = player->GetPlanet();
    auto room = player->GetRoom();
    int room_id;
    std::string input_string = std::string(input);
    std::string direction_string = Tokenizer::GetFirstToken(input_string);
    direction_string = Tokenizer::LowerCase(direction_string);
    std::string single_string = Tokenizer::GetFirstToken(input_string);
    Direction direction = BuildingState::m_direction_map[direction_string];
    switch (direction) {
        case Direction::NORTH:
            room_id = room->GetNorth();
            if(room_id == -1)
                break;
            if(single_string == "single")
                planet->GetRoom(room_id)->SetSouth(-1);
            room->SetNorth(-1);
            break;
        case Direction::SOUTH:
            room_id = room->GetSouth();
            if(room_id == -1)
                break;
            if(single_string != "single")
                planet->GetRoom(room_id)->SetNorth(-1);
            room->SetSouth(-1);
            break;
        case Direction::EAST:
            room_id = room->GetEast();
            if(room_id == -1)
                break;
            if(single_string != "single")
                planet->GetRoom(room_id)->SetWest(-1);
            room->SetEast(-1);
            break;
        case Direction::WEST:
            room_id = room->GetWest();
            if(room_id == -1)
                break;
            if(single_string != "single")
                planet->GetRoom(room_id)->SetEast(-1);
            room->SetWest(-1);
            break;
        case Direction::UP:
            room_id = room->GetUp();
            if(room_id == -1)
                break;
            if(single_string != "single")
                planet->GetRoom(room_id)->SetDown(-1);
            room->SetUp(-1);
            break;
        case Direction::DOWN:
            room_id = room->GetDown();
            if(room_id == -1)
                break;
            if(single_string != "single")
                planet->GetRoom(room_id)->SetUp(-1);
            room->SetDown(-1);
            break;
        default:
            Sender::Send("Unable to unlink room\n", connection);
            break;
    }
}

void BuildingState::CmdGetRoomID(const std::string &input, std::shared_ptr<Connection> connection,
                                 std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    std::stringstream ss;
    ss << "Room ID is " << player->GetRoomID() << Format::NL;
    Sender::Send(ss.str(), connection);
}

void BuildingState::CmdSetLongDesc(const std::string &input, std::shared_ptr<Connection> connection,
                                   std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    std::string input_string(input);
    std::replace(input_string.begin(), input_string.end(), ';', '\n');
    room->SetLongDesc(input_string);
}

void BuildingState::CmdSetShortDesc(const std::string &input, std::shared_ptr<Connection> connection,
                                    std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    room->SetShortDesc(input);
}

void BuildingState::CmdPlay(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    connection->SetState(GameStates::PLAYING, game_data);
}

void BuildingState::CmdSavePlanet(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto planet = connection->GetPlayer()->GetPlanet();
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
        {"west", Direction::WEST},
        {"up", Direction::UP},
        {"u", Direction::UP},
        {"down", Direction::DOWN},
        {"d", Direction::DOWN}
};
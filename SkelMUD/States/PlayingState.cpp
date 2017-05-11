//
// Created by nate on 11/22/15.
//

#include <sstream>
#include <math.h>
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
    if(input == "quit") {
        connection->Close();
        auto player = game_data->GetPlayer(connection->GetCharacterName());
        game_data->GetRoom(player->GetLocationID(),
                           player->GetRoomID(),
                           player->IsInShip())->RemovePlayer(player->GetID());
    }
    if(m_cmd_map.find(command) != m_cmd_map.end()) {
        m_cmd_map[command](input_string, connection, game_data);
        connection->SetPrompt(GetPrompt(connection));
    }
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
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    if (player->IsInShip()) {
        Logger::Debug("In Ship");
        auto ship = game_data->GetShip(player->GetShipID());
        if (ship->IsInOrbit()) {
            auto planet = game_data->GetPlanet(ship->GetPlanetID());
            ss << Format::CYAN << "<Orbiting planet " << planet->GetName() << ">\n";
        }
        else if (ship->IsInSpace()) {
            Utils::Coordinates coordinates = ship->GetCoordinates();
            ss << Format::CYAN << "<x: " << coordinates.x << " y: " << coordinates.y << " z: " << coordinates.z << "> \n";
        }
    }
    return ss.str();
}

void PlayingState::CmdHelp(const std::string& input, std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data)
{
    Sender::Send("'chat' to chat\n'tell <character>' to direct tell\n'online' to see who is online\n", connection);
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

void PlayingState::CmdSay(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    std::string input_string = input;
    std::stringstream ss;
    ss << Format::YELLOW << connection->GetCharacterName() << " says: " << Format::BOLD << input_string << Format::NL;
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto room = game_data->GetRoom(player->GetLocationID(),
                       player->GetRoomID(),
                       player->IsInShip());
    Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                           room->GetVisiblePlayers());

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
    // auto planet = game_data->GetPlanet(player->GetPlanetID());
    auto room = game_data->GetRoom(player->GetLocationID(),
                                   player->GetRoomID(),
                                   player->IsInShip());
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
        for (auto ship: room->GetShips()) {
            ss << ship.second->GetShipName() << Format::NL;
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
    //auto planet = game_data->GetPlanet(player->GetPlanetID());
    auto area = game_data->GetArea(player->GetLocationID(), player->IsInShip());
    int room_id = player->GetRoomID();
    int player_id = player->GetID();
    bool success = false;
    std::string arrive_string = "";
    std::string depart_string = "";
    int departed_room = 0;
    switch(direction) {
        case NORTH:
            departed_room = player->GetRoomID();
            success = area->MoveNorth(room_id, player_id);
            arrive_string = "south";
            depart_string = "north";
            break;
        case SOUTH:
            departed_room = player->GetRoomID();
            success = area->MoveSouth(room_id, player_id);
            arrive_string = "north";
            depart_string = "south";
            break;
        case EAST:
            departed_room = player->GetRoomID();
            success = area->MoveEast(room_id, player_id);
            arrive_string = "west";
            depart_string = "east";
            break;
        case WEST:
            departed_room = player->GetRoomID();
            success = area->MoveWest(room_id, player_id);
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
                                   area->GetRoom(departed_room)->GetVisiblePlayers());
            Sender::SendToMultiple(arrived_ss.str(), game_data->GetLoggedInConnections(),
                                   area->GetRoom(player->GetRoomID())->GetVisiblePlayers(connection->GetID()));
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


void PlayingState::CmdOpen(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string data = input;
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    if(player->IsInShip()) {
        auto ship = game_data->GetShip(player->GetShipID());
        if(player->GetRoomID() == 0) {
            if(!ship->IsHatchOpen()) {
                ship->OpenHatch();
                Sender::Send("Opened hatch\n", connection);
            }
            else {
                Sender::Send("Hatch already open\n", connection);
            }
        }
        else {
            Sender::Send("No hatch here\n", connection);
        }
    }
    else {
        //auto planet = game_data->GetPlanet(player->GetPlanetID());
        auto room = game_data->GetRoom(player->GetLocationID(),
                                       player->GetRoomID(),
                                       false);
        //int room_id = player->GetRoomID();
        //auto room = planet->GetRoom(room_id);
        bool found = false;
        std::string ship_name = Tokenizer::GetFirstToken(data);
        for (auto ship: room->GetShips()) {
            if (ship.second->GetShipName() == ship_name) {
                found = true;
                if (!ship.second->IsHatchOpen()) {
                    Sender::Send("Opened hatch\n", connection);
                    ship.second->OpenHatch();
                }
                else {
                    Sender::Send("Hatch is already open\n", connection);
                }
                break;
            }
        }
        if (!found) {
            Sender::Send("That ship is not here!\n", connection);
        }
    }
}

void PlayingState::CmdClose(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string data = input;
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    if(player->IsInShip()) {
        Logger::Debug("Ship ID: " + std::to_string(player->GetShipID()));
        auto ship = game_data->GetShip(player->GetShipID());
        if(player->GetRoomID() == 0) {
            if(ship->IsHatchOpen()) {
                ship->CloseHatch();
                Sender::Send("Closed hatch\n", connection);
            }
            else {
                Sender::Send("Hatch already closed\n", connection);
            }
        }
        else {
            Sender::Send("No hatch here\n", connection);
        }
    }
    else {
        auto room = game_data->GetRoom(player->GetLocationID(),
                                       player->GetRoomID(),
                                       player->IsInShip());
        bool found = false;
        std::string ship_name = Tokenizer::GetFirstToken(data);
        for (auto ship: room->GetShips()) {
            if (ship.second->GetShipName() == ship_name) {
                found = true;
                if (ship.second->IsHatchOpen()) {
                    Sender::Send("Closed hatch\n", connection);
                    ship.second->CloseHatch();
                }
                else {
                    Sender::Send("Hatch is already closed\n", connection);
                }
                break;
            }
        }
        if (!found) {
            Sender::Send("That ship is not here!\n", connection);
        }
    }
}

void PlayingState::CmdEnter(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    std::string data = input;
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    auto planet = game_data->GetPlanet(player->GetPlanetID());
    int room_id = player->GetRoomID();
    auto room = planet->GetRoom(room_id);
    bool found = false;
    std::string ship_name = Tokenizer::GetFirstToken(data);
    for(auto ship: room->GetShips()) {
        if(ship.second->GetShipName() == ship_name) {
            found = true;
            if(ship.second->IsHatchOpen()) {
                std::stringstream ss;
                ss << player->GetPlayerName() << " entered ship " << ship_name << "\n";
                Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                                       room->GetVisiblePlayers(connection->GetID()));
                room->RemovePlayer(player->GetID());
                auto ship_room = ship.second->GetRoom(0);
                ship.second->AddPlayer(player->GetID());
                ship_room->AddPlayer(player);
                player->SetShipID(ship.second->GetID());
                player->SetInShip(true);
                player->SetRoomID(0);
                std::stringstream enter_ss;
                enter_ss << player->GetPlayerName() << " entered the ship\n";
                Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                                       ship_room->GetVisiblePlayers(connection->GetID()));
                CmdLook("", connection, game_data);
            }
            else {
                Sender::Send("Hatch is closed\n", connection);
            }
            break;
        }
    }
    if(!found) {
        Sender::Send("That ship is not here!\n", connection);
    }
}

void PlayingState::CmdLeave(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    if(player->IsInShip()) {
        auto ship = game_data->GetShip(player->GetLocationID());
        if(player->GetRoomID() == 0) {
            if(ship->IsHatchOpen()) {
                auto room = ship->GetRoom(player->GetRoomID());
                std::stringstream exit_ss;
                exit_ss << player->GetPlayerName() << " left the ship\n";
                Sender::SendToMultiple(exit_ss.str(), game_data->GetLoggedInConnections(),
                                       room->GetVisiblePlayers(connection->GetID()));
                room->RemovePlayer(player->GetID());
                player->SetInShip(false);
                room = game_data->GetRoom(ship->GetPlanetID(),
                                   ship->GetRoomId(),
                                   false);
                room->AddPlayer(player);
                std::stringstream ss;
                std::string ship_name = ship->GetName();
                ss << player->GetPlayerName() << " got out of ship " << ship_name << "\n";
                Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                                       room->GetVisiblePlayers(connection->GetID()));
                CmdLook("", connection, game_data);
            }
            else {
                Sender::Send("Hatch is closed\n", connection);
            }
        }
        else {
            Sender::Send("No hatch here\n", connection);
        }
    }
    else {
        Sender::Send("Not in a vehicle\n", connection);
    }
}

void PlayingState::CmdTakeOff(const std::string &input, std::shared_ptr<Connection> connection,
                              std::shared_ptr<GameData> game_data) {
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    if(player->IsInShip()) {
        auto room = game_data->GetRoom(player->GetLocationID(),
                                       player->GetRoomID(),
                                       true);
        if(room->IsCockpit()) {
            // TODO: Skill check
            auto ship = game_data->GetShip(player->GetShipID());
            if(!ship->IsHatchOpen()) {
                if(!ship->IsInSpace()) {
                    Sender::SendToMultiple("The ship lurches as it ascends into space\n",
                                           game_data->GetLoggedInConnections(),
                                           ship->GetPlayerIDs());
                    auto planet = game_data->GetPlanet(ship->GetPlanetID());
                    ship->SetCoordinates(planet->GetCoordinates());
                    ship->SetInSpace(true);
                    ship->SetInOrbit(true);
                    planet->GetRoom(0)->RemoveShip(ship->GetID());
                }
                else
                    Sender::Send("Ship is already in space\n", connection);
            }
            else
                Sender::Send("Hatch is open\n", connection);
        }
        else
            Sender::Send("Not at a cockpit or control console\n", connection);
    }
    else
        Sender::Send("Not in a ship\n", connection);
}

void PlayingState::CmdSetCourse(const std::string &input, std::shared_ptr<Connection> connection,
                                std::shared_ptr<GameData> game_data) {
    std::string data = input;
    auto params = Tokenizer::GetAllTokens(data);
    double x = 0;
    double y = 0;
    double z = 0;
    double speed = 0;
    char* p;
    auto player = game_data->GetPlayer(connection->GetCharacterName());
    if(player->IsInShip()) {
        auto room = game_data->GetRoom(player->GetLocationID(),
                                       player->GetRoomID(),
                                       true);
        if(room->IsCockpit()) {
            // TODO: Skill check
            auto ship = game_data->GetShip(player->GetShipID());
            if(ship->IsInSpace()) {
                if(params.size() > 1 and !Utils::IsNumber(params[0])) {
                    auto planet = game_data->GetPlanet(params[0]);
                    if(planet == nullptr) {
                        Sender::Send("That planet does not exist in this system\n", connection);
                        return;
                    }
                    else {
                        Utils::Coordinates coords = planet->GetCoordinates();
                        x = coords.x;
                        y = coords.y;
                        z = coords.z;
                        speed = std::stod(params[1]);
                    }
                }
                else if(params.size() >= 4 and Utils::IsNumber(params[0])
                        and Utils::IsNumber(params[1]) and Utils::IsNumber(params[2])
                        and Utils::IsNumber(params[3])) {
                    x = std::stod(params[0]);
                    y = std::stod(params[1]);
                    z = std::stod(params[2]);
                    speed = std::stod(params[3]);
                }
                if(x == 0 and y == 0 and z == 0) {
                    Sender::Send("Course not set, bad parameters\n", connection);
                    return;
                }
                ship->SetInOrbit(false);
                Utils::Coordinates coords = ship->GetCoordinates();
                double xf = x - coords.x;
                double yf = y - coords.y;
                double zf = z - coords.z;
                double length = sqrt(xf * xf + yf * yf + zf * zf);
                Utils::Velocity velocity;
                velocity.x = (xf / length) * speed;
                velocity.y = (yf / length) * speed;
                velocity.z = (zf / length) * speed;
                ship->SetVelocity(velocity);
                std::stringstream ss;
                ss << "Course set for " << "X: " << x << "Y: " << y
                    << "Z: " << z << " at speed " << speed << "\n";
                Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                                       ship->GetPlayerIDs());
            }
            else
                Sender::Send("Ship is not in space\n", connection);
        }
        else
            Sender::Send("Not at a cockpit or control console\n", connection);
    }
    else
        Sender::Send("Not in a ship\n", connection);
}
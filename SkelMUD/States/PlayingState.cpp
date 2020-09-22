//
// Created by nate on 11/22/15.
//

#include <sstream>
#include <math.h>
#include <algorithm>
#include <random>
#include "PlayingState.h"
#include "../Tokenizer.h"
#include "../Sender.h"
#include "../Format.h"
#include "../Logger.h"
#include "StateFactory.h"
#include "../Areas/Room.h"
#include "../GameData.h"
#include "../Ships/Ship.h"
#include "../Items/Consumable.h"
#include "../Items/Wieldable.h"

void PlayingState::processInput(const std::string &input, std::shared_ptr<Connection> connection) {
    std::string input_string = input;
    std::string command = Tokenizer::GetFirstToken(input_string);
    command = Tokenizer::LowerCase(command);
    command = Utils::FindMatch(m_cmd_vect, command);
    if (m_cmd_map.find(command) != m_cmd_map.end()) {
        m_cmd_map[command](input_string, connection, game_data);
    }
    else
        Sender::Send("Unrecognized command!\n", connection);
}

void PlayingState::init(std::shared_ptr<Connection> connection) {
    std::stringstream ss;
    // Check if a connection is already using this character
    auto player = connection->GetPlayer();
    auto existing_conn = game_data->GetConnection(player->GetPlayerName());
    if (existing_conn != nullptr) {
        existing_conn->GetState()->Shutdown(existing_conn);
        player->GetRoom()->AddPlayer(player);
    }
    else {
        // TODO: set player to proper planet
        game_data->GetPlanet(0)->GetRoom(0)->AddPlayer(player);
        //TODO: set proper planet id
        player->SetPlanet(game_data->GetPlanet(0));
        player->SetRoomID(0);
        player->SetRoom(game_data->GetPlanet(0)->GetRoom(0));
    }
    player->SetID(connection->GetID());
    ss << "Welcome to SkelMUD, " << connection->GetCharacterName() << "!" << Format::NL;
    Sender::Send(ss.str(), connection);
    CleanPrompt(*connection);
    connection->SetLoggedIn(true);
//    player->SetVisible(true);
    CmdLook("", connection, game_data);
    ss.str(std::string());
    ss.clear();
    ss << connection->GetCharacterName() << " has logged in!\n";
    Sender::SendAll(ss.str(), game_data->GetLoggedInConnections(), connection->GetSocket());
}

std::string PlayingState::GetPrompt(Connection connection) {
    auto player = connection.GetPlayer();
    std::stringstream ss;
    // ss << Format::YELLOW << "<" + Format::BLUE << connection.GetCharacterName() << Format::YELLOW << "> ";
    ss << Format::RED << "<HP: " << player->GetHP() << "/" << player->GetMaxHP() << "> ";
    ss << Format::BLUE << "<SP: " << player->GetSP() << "/" << player->GetMaxSP() << "> ";
    ss << Format::YELLOW << "<Stam: " << player->GetStamina() << "/" << player->GetMaxStamina() << "> " << Format::NL;
    if (player->IsInShip()) {
        auto ship = player->GetShip();
        if (ship->IsInOrbit()) {
            auto planet = ship->GetPlanet();
            ss << Format::CYAN << "<Orbiting planet " << planet->GetName() << ">\n";
        }
        else if (ship->IsInSpace()) {
            Utils::Vector3 coordinates = ship->GetCoordinates();
            ss << Format::CYAN << "<x: " << coordinates.x << " y: " << coordinates.y << " z: " << coordinates.z <<
            "> \n";
        }
    }
    return ss.str();
}

void PlayingState::BeginCombat(std::shared_ptr<Player> player,
                               std::shared_ptr<Player> target) {
    player->BeginFighting(target);
    if(!target->IsFighting()) {
        target->BeginFighting(player);
    }
    player->AddAttacker(target);
    target->AddAttacker(player);
}

void PlayingState::CmdHelp(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::stringstream ss;
    ss << "'chat' to chat\n'tell <character>' to direct tell\n'online' to see who is online\n";
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdTell(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string input_string = input;
    std::string target = Tokenizer::GetFirstToken(input_string);
    auto target_connection = game_data->GetConnection(target);
    if (target_connection == nullptr) {
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
    Sender::SendAll(ss.str(), game_data->GetLoggedInConnections());
}

void PlayingState::CmdSay(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    std::string input_string = input;
    std::stringstream ss;
    ss << Format::YELLOW << connection->GetCharacterName() << " says: " << Format::BOLD << input_string << Format::NL;
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                           room->GetVisiblePlayers());

}

void PlayingState::CmdEmote(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    std::string input_string = input;
    std::stringstream ss;
    ss << Format::BLUE << Format::BOLD << connection->GetCharacterName()
    << " " << Format::BOLD << input_string << Format::NL;
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                           room->GetVisiblePlayers());

}

void PlayingState::CmdOnline(const std::string &input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data) {
    std::stringstream ss;
    ss << Format::BOLD << Format::YELLOW << "Players Online:" << Format::NL << Format::BLUE;
    for (auto conn: game_data->GetLoggedInConnections()) {
        ss << conn.second->GetCharacterName() << Format::NL;
    }
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdLook(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    std::stringstream ss;
    ss << Format::NL << Format::BOLD << Format::YELLOW << room->GetShortDescription() << Format::RESET << Format::NL;
    ss << room->GetLongDescription() << Format::NL;
    ss << Format::BOLD << Format::CYAN << "Directions:" << Format::NL << GetValidDirections(*room) << Format::NL
    << Format::RESET;
    auto other_players = room->GetVisiblePlayerNames(player->GetPlayerName());
    for (auto other_player: other_players) {
        ss << Format::BOLD << Format::BLUE << other_player << " is here." << Format::RESET << Format::NL;
    }
    for (auto npc_str : room->GetNPCs()) {
        auto npc = game_data->GetNPC(npc_str);
        ss << npc->GetName() << " (NPC) is here." << Format::RESET << Format::NL;
    }
    if(room->GetItems().size() > 0) {
        ss << Format::MAGENTA << "Items:" << Format::NL;
        for (auto item : room->GetItems()) {
            ss << item.first << ": " << item.second << Format::NL;
        }
        ss << Format::RESET;
    }
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdShips(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto room = connection->GetPlayer()->GetRoom();
    std::stringstream ss;
    if (room->IsLandable()) {
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

void PlayingState::CmdUp(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data) {
    Move(connection, game_data, Direction::UP);
}

void PlayingState::CmdDown(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    Move(connection, game_data, Direction::DOWN);
}

void PlayingState::Move(std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data,
                        Direction direction) {
    auto player = connection->GetPlayer();
    if(player->IsFighting()) {
        if(!AttemptEscape(player))
            return;
    }
    auto area = player->GetLocation();
    auto departed_room = player->GetRoom();
    std::string arrive_string = "";
    std::string depart_string = "";
    int new_room_int = -1;
    switch (direction) {
        case NORTH:
            new_room_int = departed_room->GetNorth();
            arrive_string = "the south";
            depart_string = "to the north";
            break;
        case SOUTH:
            new_room_int = departed_room->GetSouth();
            arrive_string = "the north";
            depart_string = "to the south";
            break;
        case EAST:
            new_room_int = departed_room->GetEast();
            arrive_string = "the west";
            depart_string = "to the east";
            break;
        case WEST:
            new_room_int = departed_room->GetWest();
            arrive_string = "the east";
            depart_string = "to the west";
            break;
        case UP:
            new_room_int = departed_room->GetUp();
            arrive_string = "above";
            depart_string = "downwards";
            break;
        case DOWN:
            new_room_int = departed_room->GetDown();
            arrive_string = "below";
            depart_string = "upwards";
            break;
        default:
            Logger::Error("Unhandled Direction");
    }
    if (new_room_int != -1) {
        area->ChangeRoom(departed_room->GetID(), new_room_int, player);
        if (player->IsVisible()) {
            std::stringstream departed_ss;
            std::stringstream arrived_ss;
            std::string player_name = player->GetPlayerName();
            departed_ss << player_name << " has left " << depart_string << Format::NL;
            arrived_ss << player_name << " has arrived from " << arrive_string << Format::NL;
            Sender::SendToMultiple(departed_ss.str(), game_data->GetLoggedInConnections(),
                                   departed_room->GetVisiblePlayers());
            auto new_room = game_data->GetRoom(area->GetID(), new_room_int, player->IsInShip());
            Sender::SendToMultiple(arrived_ss.str(), game_data->GetLoggedInConnections(),
                                   new_room->GetVisiblePlayers(connection->GetID()));
        }
        player->RegenStam(-2);
        CmdLook("", connection, game_data);
    }
    else
        Sender::Send("Can't go that way!\n", connection);
}

void PlayingState::CmdBuild(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    if (connection->GetAccount().GetAccountLevel() > Account::NORMAL)
        connection->SetState(GameStates::BUILDING, game_data);
    else
        Sender::Send("Not Authorized to enter build mode\n", connection);
}

std::string PlayingState::GetValidDirections(Room &room) {
    std::stringstream ss;
    if (room.GetNorth() != -1)
        ss << "north ";
    if (room.GetSouth() != -1)
        ss << "south ";
    if (room.GetEast() != -1)
        ss << "east ";
    if (room.GetWest() != -1)
        ss << "west ";
    if (room.GetUp() != -1)
        ss << "up ";
    if (room.GetDown() != -1)
        ss << "down ";
    ss << "\n";
    return ss.str();
}

void PlayingState::CmdGoto(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string input_string = std::string(input);
    std::string room_number_string = Tokenizer::GetFirstToken(input_string);
    int newroom = 0;
    if (Utils::IsNumber(room_number_string)) {
        newroom = std::stoi(room_number_string);
    }
    auto player = connection->GetPlayer();
    Escape(player);
    auto planet = player->GetPlanet();
    if (planet->ChangeRoom(player->GetRoomID(), newroom, player)) {
        CmdLook("", connection, game_data);
    }
    else {
        Sender::Send("Room does not exist!\n", connection);
    }
}

void PlayingState::CmdOpen(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string data = input;
    auto player = connection->GetPlayer();
    if (player->IsInShip()) {
        auto ship = player->GetShip();
        if (player->GetRoomID() == 0) {
            if (!ship->IsInSpace()) {
                if (!ship->IsHatchOpen()) {
                    ship->OpenHatch();
                    Sender::Send("Opened hatch\n", connection);
                }
                else {
                    Sender::Send("Hatch already open\n", connection);
                }
            }
            else
                Sender::Send("Ship is in space, can't open hatch!\n", connection);
        }
        else {
            Sender::Send("No hatch here\n", connection);
        }
    }
    else {
        auto room = player->GetRoom();
        bool found = false;
//        std::string ship_name = Tokenizer::GetFirstToken(data);
        auto ships = room->GetShips();
        auto vals = Utils::ExtractMapValues(ships);
        std::vector<std::string> ship_names;
        for(auto val: vals) {
            ship_names.push_back(val->GetShipName());
        }
        std::sort(ship_names.begin(), ship_names.end());
        std::string ship_name = Utils::FindMatch(ship_names, data);
        for (auto ship: ships) {
            if (ship.second->GetShipName() == ship_name) {
                found = true;
                if (!ship.second->IsHatchOpen()) {
                    std::stringstream ss;
                    ss << "Opened hatch on " << ship_name << "\n";
                    Sender::Send(ss.str(), connection);
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
    auto player = connection->GetPlayer();
    if (player->IsInShip()) {
        auto ship = player->GetShip();
        if (player->GetRoomID() == 0) {
            if (ship->IsHatchOpen()) {
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
        auto room = player->GetRoom();
        bool found = false;
//    std::string ship_name = Tokenizer::GetFirstToken(data);
        auto ships = room->GetShips();
        auto vals = Utils::ExtractMapValues(ships);
        std::vector<std::string> ship_names;
        for(auto val: vals) {
            ship_names.push_back(val->GetShipName());
        }
        std::sort(ship_names.begin(), ship_names.end());
        std::string ship_name = Utils::FindMatch(ship_names, data);
        for (auto ship: ships) {
            if (ship.second->GetShipName() == ship_name) {
                found = true;
                if (ship.second->IsHatchOpen()) {
                    std::stringstream ss;
                    ss << "Closed hatch on " << ship_name << "\n";
                    Sender::Send(ss.str(), connection);
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
    auto player = connection->GetPlayer();
    if(player->IsFighting()) {
        if(!AttemptEscape(player))
            return;
    }
    auto room = player->GetRoom();
    bool found = false;
    auto ships = room->GetShips();
    auto vals = Utils::ExtractMapValues(ships);
    std::vector<std::string> ship_names;
    for(auto val: vals) {
        ship_names.push_back(val->GetShipName());
    }
    std::sort(ship_names.begin(), ship_names.end());
    std::string ship_name = Utils::FindMatch(ship_names, data);
    for (auto ship: ships) {
        if (ship.second->GetShipName() == ship_name) {
            found = true;
            if (ship.second->IsHatchOpen()) {
                std::stringstream ss;
                ss << player->GetPlayerName() << " entered ship " << ship_name << "\n";
                Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                                       room->GetVisiblePlayers(connection->GetID()));
                room->RemovePlayer(player->GetID());
                auto ship_room = ship.second->GetRoom(0);
                ship.second->AddPlayer(player->GetID());
                ship_room->AddPlayer(player);
                player->SetShip(ship.second);
                player->SetInShip(true);
                player->SetRoom(ship_room);
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
    if (!found) {
        Sender::Send("That ship is not here!\n", connection);
    }
}

void PlayingState::CmdLeave(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    if(player->IsFighting()) {
        if(!AttemptEscape(player))
            return;
    }
    if (player->IsInShip()) {
        auto ship = player->GetShip();
        if (player->GetRoomID() == 0) {
            if (ship->IsHatchOpen()) {
                auto room = player->GetRoom();
                std::stringstream exit_ss;
                exit_ss << player->GetPlayerName() << " left the ship\n";
                Sender::SendToMultiple(exit_ss.str(), game_data->GetLoggedInConnections(),
                                       room->GetVisiblePlayers(connection->GetID()));
                room->RemovePlayer(player->GetID());
                player->SetInShip(false);
                // TODO
                room = ship->GetPlanet()->GetRoom(0);
                // TODO
                player->SetPlanet(ship->GetPlanet());
                player->SetRoom(room);
                room->AddPlayer(player);
                ship->RemovePlayer(player->GetID());
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
    auto player = connection->GetPlayer();
    if (CheckCockpitCommand(connection, game_data, false)) {
        auto ship = player->GetShip();
        if (!ship->IsHatchOpen()) {
            Sender::SendToMultiple("The ship lurches as it ascends into space\n",
                                   game_data->GetLoggedInConnections(),
                                   ship->GetPlayerIDs());
            auto planet = ship->GetPlanet();
            ship->SetCoordinates(planet->GetCoordinates());
            ship->SetInSpace(true);
            ship->SetInOrbit(true);
            auto room = planet->GetRoom(0);
            room->RemoveShip(ship->GetID());
            std::stringstream ss;
            ss << ship->GetName() << " departs the planet\n";
            Sender::SendToMultiple(ss.str(),
                                   game_data->GetLoggedInConnections(),
                                   room->GetPlayers());
        }
        else
            Sender::Send("Hatch is open\n", connection);
    }
}

void PlayingState::CmdSetCourse(const std::string &input, std::shared_ptr<Connection> connection,
                                std::shared_ptr<GameData> game_data) {
    std::string data = input;
    auto params = Tokenizer::GetAllTokens(data);
    double x = 0;
    double y = 0;
    double z = 0;
    auto player = connection->GetPlayer();
    bool course_set = false;
    auto ship = player->GetShip();
    if (CheckCockpitCommand(connection, game_data, true)) {
        // TODO: Skill check

        if (params.size() > 0 and !Utils::IsNumber(params[0])) {
            auto planet = game_data->GetPlanet(params[0]);
            if (planet == nullptr) {
                Sender::Send("That planet does not exist in this system\n", connection);
                return;
            }
            else {
                Utils::Vector3 coords = planet->GetCoordinates();
                x = coords.x;
                y = coords.y;
                z = coords.z;
                if (params.size() > 1 and Utils::IsNumber(params[1])) {
                    ship->SetSpeed(std::stod(params[1]));
                }
                course_set = true;
            }
        }
        else if (params.size() >= 3 and Utils::IsNumber(params[0])
                 and Utils::IsNumber(params[1]) and Utils::IsNumber(params[2])) {
            x = std::stod(params[0]);
            y = std::stod(params[1]);
            z = std::stod(params[2]);
            if (params.size() >= 4 and Utils::IsNumber(params[3])) {
                ship->SetSpeed(std::stod(params[3]));
            }
            course_set = true;
        }
        if (!course_set) {
            Sender::Send("Course not set, bad parameters\n", connection);
            return;
        }
        ship->SetDestination(x, y, z);
        ship->SetInOrbit(false);
        ChangeSpeed(ship->GetSpeed(), ship);
        std::stringstream ss;
        ss << "Course set for " << "X: " << x << " Y: " << y
        << " Z: " << z << " at speed " << ship->GetSpeed() << "\n";
        Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                               ship->GetPlayerIDs());
    }
}

void PlayingState::ChangeSpeed(double speed, std::shared_ptr<Ship> &ship) {
    Utils::Vector3 coords = ship->GetCoordinates();
    Utils::Vector3 destination = ship->GetDestination();
    double xf = destination.x - coords.x;
    double yf = destination.y - coords.y;
    double zf = destination.z - coords.z;
    double length = sqrt(xf * xf + yf * yf + zf * zf);
    if (length == 0)
        return;
    Utils::Vector3 velocity;
    velocity.x = (xf / length) * speed;
    velocity.y = (yf / length) * speed;
    velocity.z = (zf / length) * speed;
    ship->SetVelocity(velocity);
    ship->SetInOrbit(false);
}

void PlayingState::CmdSetSpeed(const std::string &input, std::shared_ptr<Connection> connection,
                               std::shared_ptr<GameData> game_data) {
    if (CheckCockpitCommand(connection, game_data, true)) {
        std::string input_data = input;
        std::string speed_string = Tokenizer::GetFirstToken(input_data);
        auto player = connection->GetPlayer();
        auto ship = player->GetShip();
        if (Utils::IsNumber(speed_string)) {
            ship->SetSpeed(std::stod(speed_string));
            ChangeSpeed(std::stod(speed_string), ship);
            std::stringstream ss;
            ss << "Ship's speed set to " << speed_string << Format::NL;
            Sender::SendToMultiple(ss.str(), game_data->GetLoggedInConnections(),
                                   ship->GetPlayerIDs());
        }
        else {
            Sender::Send("Invalid speed parameter", connection);
        }
    }
}

void PlayingState::CmdScan(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    if (CheckCockpitCommand(connection, game_data, true)) {
        std::stringstream ss;
        auto planets = game_data->GetPlanets().GetPlanets();
        auto ships = game_data->GetShips().GetShips();
        ss << "Planets:\n";
        for (auto planet : planets) {
            std::string planet_name = planet.second->GetName();
            double x = planet.second->GetCoordinates().x;
            double y = planet.second->GetCoordinates().y;
            double z = planet.second->GetCoordinates().z;
            ss << planet_name << " X: " << x << " Y: " << y << " Z: " << z << "\n";
        }
        ss << Format::NL;
        ss << "Ships:\n";
        for (auto ship : ships) {
            if (ship.second->IsInSpace()) {
                std::string ship_name = ship.second->GetName();
                double x = ship.second->GetCoordinates().x;
                double y = ship.second->GetCoordinates().y;
                double z = ship.second->GetCoordinates().z;
                ss << ship_name << " X: " << x << " Y: " << y << " Z: " << z << "\n";

            }
        }
        Sender::Send(ss.str(), connection);
    }
}

void PlayingState::CmdOrbit(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto ship = player->GetShip();
    if (CheckCockpitCommand(connection, game_data, true)) {
        for (auto planet : game_data->GetPlanets().GetPlanets()) {
            if (Utils::GetDistance(planet.second->GetCoordinates(),
                                   ship->GetCoordinates()) <= 100) {
                ship->SetPlanetId(planet.second->GetID());
                ship->SetPlanet(planet.second);
                ship->SetInOrbit(true);
                ship->SetVelocity(0, 0, 0);
                std::stringstream ss;
                ss << "Entered orbit around " << planet.second->GetName() << "\n";
                Sender::SendToMultiple(ss.str(),
                                       game_data->GetLoggedInConnections(),
                                       ship->GetPlayerIDs());
                return;
            }
        }
        Sender::Send("Not in range of any planet\n", connection);
    }
}

void PlayingState::CmdLand(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    if (CheckCockpitCommand(connection, game_data, true)) {
        auto player = connection->GetPlayer();
        auto ship = player->GetShip();
        if (ship->IsInOrbit()) {
            auto planet = ship->GetPlanet();
            ship->SetInSpace(false);
            ship->SetInOrbit(false);
            auto room = planet->GetRoom(0);
            if (!room) {
                // error handle
                return;
            }
            room->AddShip(ship);

            std::stringstream ss;
            ss << "The ship has landed on " << planet->GetName() << "\n";
            Sender::SendToMultiple(ss.str(),
                                   game_data->GetLoggedInConnections(),
                                   ship->GetPlayerIDs());
            std::stringstream ss2;
            ss2 << ship->GetName() << " has arrived\n";
            Sender::SendToMultiple(ss2.str(),
                                   game_data->GetLoggedInConnections(),
                                   room->GetPlayers());
        }
        else {
            Sender::Send("Must be in orbit before you can land\n", connection);
        }
    }
}

bool PlayingState::CheckCockpitCommand(std::shared_ptr<Connection> connection,
                                       std::shared_ptr<GameData> game_data, bool InSpace) {
    auto player = connection->GetPlayer();
    std::stringstream ss;
    if (player->IsInShip()) {
        auto ship = player->GetShip();
        if (player->GetRoom()->IsCockpit()) {
            if (!ship->IsInSpace() xor InSpace) {
                return true;
            }
            else {
                if (ship->IsInSpace())
                    ss << "Ship is in space\n";
                else
                    ss << "Ship is not in space\n";
            }
        }
        else {
            ss << "Not at a cockpit or control console\n";
        }
    }
    else {
        ss << "Not in a ship\n";
    }
    Sender::Send(ss.str(), connection);
    return false;
}

void PlayingState::CmdQuit(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    connection->Close();
    auto player = connection->GetPlayer();
    player->GetRoom()->RemovePlayer(player->GetID());
    if (player->IsInShip()) {
        player->GetShip()->RemovePlayer(player->GetID());
        player->SetInShip(false);
    }
    // TODO
    //game_data->GetRoom(player->GetPlanet()->GetID(), 0, false)->AddPlayer(player);
}

void PlayingState::Shutdown(std::shared_ptr<Connection> connection) {
    CmdQuit("", connection, game_data);
}

void PlayingState::CmdInventory(const std::string &input, std::shared_ptr<Connection> connection,
                                std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    std::stringstream ss;
    ss << Format::RED << Format::BOLD << "Wielding:" << Format::NORMAL << Format::NL;
    auto main_hand = player->GetMainHand();
    auto off_hand = player->GetOffHand();
    ss << "Main Hand: ";
    if(main_hand != nullptr)
        ss << main_hand->GetItemName() << Format::NL;
    else
        ss << "None" << Format::NL;
    ss << "Off Hand: ";
    if(off_hand != nullptr)
        ss << off_hand->GetItemName() << Format::NL;
    else
        ss << "None" << Format::NL;
    ss << Format::NL << Format::CYAN << Format::BOLD << "Inventory:" << Format::NORMAL << Format::NL;
    for(auto item : player->GetItems()) {
        ss << item.first << ": " << item.second << Format::NL;
    }
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdGet(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    std::string input_data = input;
//    std::string item_name = Tokenizer::GetFirstToken(input_data);
    std::string item_name = input_data;
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    auto items = room->GetItems();
    std::vector<std::string> keys = Utils::ExtractMapKeys(items);
    std::sort(keys.begin(), keys.end());
    item_name = Utils::FindMatch(keys, item_name);
    if(items.find(item_name) != items.end()) {
        player->AddItem(item_name);
        room->RemoveItem(item_name);
        std::stringstream ss;
        ss << "Picked up " << item_name << Format::NL;
        Sender::Send(ss.str(), connection);
    }
    else {
        Sender::Send("That item isn't here!\r\n", connection);
    }
}

void PlayingState::CmdDrop(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    std::string input_data = input;
//    std::string item_name = Tokenizer::GetFirstToken(input_data);
    std::string item_name = input_data;
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    auto items = player->GetItems();
    std::vector<std::string> keys = Utils::ExtractMapKeys(items);
    std::sort(keys.begin(), keys.end());
    item_name = Utils::FindMatch(keys, input_data);
    if(items.find(item_name) != items.end()) {
        room->AddItem(item_name);
        player->RemoveItem(item_name);
        std::stringstream ss;
        ss << "Dropped " << item_name << Format::NL;
        Sender::Send(ss.str(), connection);
    }
    else {
        Sender::Send("You don't have that item!\r\n", connection);
    }
}

void PlayingState::CmdStats(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    std::string p_name = player->GetPlayerName();
    std::string p_race = player->GetPlayerRaceStr();
    std::string p_class = player->GetPlayerClass();
    std::stringstream ss;
    ss << p_name << " the " << p_race << " " << p_class << Format::NL;
    ss << "Credits       : " << player->GetCredits() << Format::NL;
    ss << "Strength      : " << player->GetNetStrength() << Format::NL;
    ss << "Endurance     : " << player->GetNetEndurance() << Format::NL;
    ss << "Intelligence  : " << player->GetNetIntelligence() << Format::NL;
    ss << "Dexterity     : " << player->GetNetDexterity() << Format::NL;
    ss << "Skill         : " << player->GetNetSkill() << Format::NL;
    ss << "Points remaining: " << player->GetAttributePoints() << Format::NL;
    Sender::Send(ss.str(), connection);
}


void PlayingState::CmdAttack(const std::string &input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    auto players = room->GetVisiblePlayerNames(player->GetID());
    auto match = Utils::FindMatch(players, input);
    if(std::find(players.begin(), players.end(), match) != players.end()) {
        auto target = game_data->GetPlayer(match);
        std::stringstream sst;
        sst << Format::RED << "Attacked by " << player->GetName() << "!" << Format::NL;
        Sender::Send(sst.str(), game_data->GetConnection(match));
        std::stringstream ss;
        ss << Format::YELLOW << "You attack " << match << "!" << Format::NL;
        Sender::Send(ss.str(), connection);
        BeginCombat(player, target);
    }
    else {
        std::stringstream ss;
        ss << "No " << input << "here to attack" << Format::NL;
        Sender::Send(ss.str(), connection);
    }
}

void PlayingState::CmdUse(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    Use(input, connection, game_data, NONE);
}

void PlayingState::CmdEat(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    Use(input, connection, game_data, EAT);
}

void PlayingState::CmdDrink(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    Use(input, connection, game_data, DRINK);
}

void PlayingState::CmdWield(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto items = Utils::ExtractMapKeys(player->GetItems());
    std::string match = Utils::FindMatch(items, input);
    auto item = game_data->GetItem(match);
    std::stringstream ss;
    if(item == nullptr) {
        ss << "You do not carry that item!" << Format::NL;
        player->Send(ss.str());
        return;
    }
    auto mixin = item->GetMixin("Wieldable");
    if(mixin == nullptr) {
        ss << item->GetItemName() << " can not be wielded!" << Format::NL;
        player->Send(ss.str());
        return;
    }
    auto wieldable = std::dynamic_pointer_cast<Wieldable>(mixin);
    auto hand = wieldable->GetHandType();
    if(hand == MAIN_HAND) {
        auto main = player->GetMainHand();
        std::stringstream ss;
        if (main != nullptr) {
            player->AddItem(main->GetItemName());
            ss << "Removed " << main->GetItemName() << Format::NL;
        }
        player->SetMainHand(item);
        player->RemoveItem(item->GetItemName());
        ss << "Wielding " << item->GetItemName() << Format::NL;
        player->Send(ss.str());
    }
    else if(hand == OFF_HAND) {
        auto off = player->GetOffHand();
        auto main = player->GetMainHand();
        if(off != nullptr) {
            player->AddItem(off->GetItemName());
            ss << "Removed " << off->GetItemName() << Format::NL;
        }
        else if(main != nullptr) {
            auto main_hand = std::dynamic_pointer_cast<Wieldable>(main->GetMixin("Wieldable"));
            if (main_hand->GetHandType() == TWO_HAND) {
                ss << "Remove two handed weapon before equipping!" << Format::NL;
                player->Send(ss.str());
                return;
            }
        }
        player->SetOffHand(item);
        player->RemoveItem(item->GetItemName());
        ss << "Wielding " << item->GetItemName() << Format::NL;
    }
    else if(hand == EITHER_HAND) {
        auto main = player->GetMainHand();
        auto off = player->GetOffHand();
        if(main != nullptr) {
            auto main_hand = std::dynamic_pointer_cast<Wieldable>(main->GetMixin("Wieldable"));
            if(off == nullptr and main_hand->GetHandType() != TWO_HAND) {
                player->SetOffHand(item);
                player->RemoveItem(item->GetItemName());
                ss << "Wielding " << item->GetItemName() << " in off hand" << Format::NL;
            }
            else {
                player->SetMainHand(item);
                player->RemoveItem(item->GetItemName());
                player->AddItem(main->GetItemName());
                ss << "Unwielded " << main->GetItemName() << Format::NL;
                ss << "Wielding " << item->GetItemName() << " in main hand" << Format::NL;
            }
        }
        else {
            player->SetMainHand(item);
            player->RemoveItem(item->GetItemName());
            ss << "Wielding " << item->GetItemName() << " in main hand" << Format::NL;
        }
    }
    else if(hand == TWO_HAND) {
        auto main = player->GetMainHand();
        auto off = player->GetOffHand();
        if(main != nullptr) {
            player->AddItem(main->GetItemName());
            ss << "Unwielded " << main->GetItemName() << Format::NL;
        }
        if(off != nullptr) {
            player->AddItem(off->GetItemName());
            player->SetOffHand(nullptr);
            ss << "Unwielded " << off->GetItemName() << Format::NL;
        }
        player->SetMainHand(item);
        player->RemoveItem(item->GetItemName());
        ss << "Wielding " << item->GetItemName() << " in main hand" << Format::NL;
    }
    player->Send(ss.str());
}

void PlayingState::CmdUnWield(const std::string &input, std::shared_ptr<Connection> connection,
                              std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto match = Utils::FindMatch({"main", "off"}, input);
    std::stringstream ss;
    if(match == "main") {
        auto main = player->GetMainHand();
        if (main != nullptr) {
            player->SetMainHand(nullptr);
            player->AddItem(main);
            ss << "Unwielded " << main->GetItemName() << Format::NL;
        } else {
            ss << "Nothing to unwield" << Format::NL;
        }
    }
    else if(match == "off") {
        auto off = player->GetOffHand();
        if(off != nullptr) {
            player->SetOffHand(nullptr);
            player->AddItem(off);
            ss << "Unwielded " << off->GetItemName() << Format::NL;
        } else {
            ss << "Nothing to unwield" << Format::NL;
        }
    }
    else {
        ss << "Please indicate <main> or <off> hand" << Format::NL;
    }
    player->Send(ss.str());
}

void PlayingState::CmdAdvancedPrompt(const std::string &input, std::shared_ptr<Connection> connection,
                                    std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    std::stringstream ss;
    ss << "Advanced Prompt ";
    if(input == "off") {
        connection->AdvancedPrompt(false);
        ss << "off" << Format::NL;
        player->Send(ss.str());
    } else {
        connection->AdvancedPrompt(true);
        ss << "on" << Format::NL;
        player->Send(ss.str());
    }
    connection->AdvancedPrompt(input != "off");
}

void PlayingState::CmdStore(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    for(auto npc_str: room->GetNPCs()) {
        auto npc = game_data->GetNPC(npc_str);
        if(npc->IsShopKeeper()) {
            std::stringstream ss;
            ss << npc->GetName() <<"'s wares" << Format::NL;
            auto sk = std::dynamic_pointer_cast<ShopKeeper>(npc->GetMixin("Shopkeeper"));
            for(auto item : sk->GetItems()) {
                ss << item.first << " Quantity: " << item.second << " Price: ";
                ss << sk->GetBuyCost(game_data->GetItem(item.first)->GetValue()) << Format::NL;
            }
            Sender::Send(ss.str(), connection);
        } else {
            Sender::Send("No Shopkeeper NPCs here!\r\n", connection);
        }
    }
}

void PlayingState::CmdBuy(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    for(auto const &npc_str: room->GetNPCs()) {
        auto npc = game_data->GetNPC(npc_str);
        if(npc->IsShopKeeper()) {
            std::stringstream ss;
            auto sk = std::dynamic_pointer_cast<ShopKeeper>(npc->GetMixin("Shopkeeper"));
            auto items = Utils::ExtractMapKeys(sk->GetItems());
            std::string match = Utils::FindMatch(items, input);
            // check if enough credits
            int price = sk->GetBuyCost(game_data->GetItem(match)->GetValue());
            if(price > player->GetCredits())
            {
                ss << "You do not have enough credits to buy " << match << "!" << Format::NL;
            }
            else {
                sk->RemoveItem(match);
                player->AddItem(match);
                player->RemoveCredits(price);
                ss << "You bought " << match << " for " << price << " credits." << Format::NL;
            }
            Sender::Send(ss.str(), connection);
        } else {
            Sender::Send("No Shopkeeper NPCs here!\r\n", connection);
        }
    }
}

void PlayingState::CmdSell(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    for(auto const &npc_str: room->GetNPCs()) {
        auto npc = game_data->GetNPC(npc_str);
        if(npc->IsShopKeeper()) {
            std::stringstream ss;
            auto sk = std::dynamic_pointer_cast<ShopKeeper>(npc->GetMixin("Shopkeeper"));
            auto items = Utils::ExtractMapKeys(player->GetItems());
            std::string match = Utils::FindMatch(items, input);
            int price = sk->GetSellCost(game_data->GetItem(match)->GetValue());
            sk->AddItem(match);
            player->RemoveItem(match);
            player->AddCredits(price);
            ss << "You sold " << match << " for " << price << " credits." << Format::NL;
            Sender::Send(ss.str(), connection);
        } else {
            Sender::Send("No Shopkeeper NPCs here!\r\n", connection);
        }
    }
}

void PlayingState::Use(const std::string &input, const std::shared_ptr<Connection> &connection,
                       const std::shared_ptr<GameData> &game_data, Consume use_type) {
    auto player = connection->GetPlayer();
    auto items = player->GetItems();
    auto keys = Utils::ExtractMapKeys(items);
    auto match = Utils::FindMatch(keys, input);
    auto item = game_data->GetItem(match);
    if(item != nullptr) {
        std::stringstream ss;
        if(item->HasMixin("Consumable")) {
            auto mixin = item->GetMixin("Consumable");
            auto consumable = std::dynamic_pointer_cast<Consumable>(mixin);
            if(use_type == EAT) {
                std::string eat = consumable->GetEat();
                if(eat.empty()) {
                    ss << eat << Format::NL;
                    player->Send(ss.str());
                }
                else {
                    ss << "You can't eat the " << item->GetItemName() << "!" << Format::NL;
                    player->Send(ss.str());
                    return;
                }
            }
            else if(use_type == DRINK) {
                std::string drink = consumable->GetDrink();
                if(drink.empty()) {
                    ss << drink << Format::NL;
                    player->Send(ss.str());
                }
                else {
                    ss << "You can't drink the " << item->GetItemName() << "!" << Format::NL;
                    player->Send(ss.str());
                    return;
                }
            }
            else {
                ss << "You use the " << item->GetItemName() << "." << Format::NL;
                player->Send(ss.str());
            }
            UseConsumable(consumable, player);
            player->RemoveItem(match);
        }
        else {
            ss << "You can't ";
            if(use_type == EAT)
                ss << "eat";
            else if(use_type == DRINK)
                ss << "drink";
            else
                ss << "use";
            ss << " the " << item->GetItemName() << "!" << Format::NL;
            player->Send(ss.str());
        }
    }
}

void PlayingState::UseConsumable(const std::shared_ptr<Consumable> &consumable, const std::shared_ptr<Player> &player) {
    int hp = consumable->GetHP();
    if(hp != 0) {
        player->Heal(hp);
    }
}

void PlayingState::Escape(std::shared_ptr<Player> player) {
    auto target = player->GetTarget();
    player->StopFighting();
    for(auto const &attacker: player->GetAttackers()) {
        attacker->StopFighting();
        attacker->RemoveAttacker(player);
        if(attacker->IsAttacked()) {
            attacker->BeginFighting(attacker->GetAttackers()[0]);
        }
    }
    player->RemoveAttacker(target);
}

bool PlayingState::AttemptEscape(std::shared_ptr<Player> player) {
    // TODO: implement real attempt to run
    auto attackers = player->GetAttackers();
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<long> dist(1, 10 + attackers.size());
    if (dist(mt) > 8 + attackers.size()) {
        auto target = player->GetTarget();
        player->Send("Escaped!\n");
        if (target != nullptr)
            target->Send(player->GetName() + " escaped!\n");
        Escape(player);
    } else {
        player->Send("Failed to escape!\n");
        return false;
    }
    return true;
}

void PlayingState::BuildCommandVector() {
    m_cmd_vect = Utils::ExtractMapKeys(m_cmd_map);
}

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
#include "StateFactory.h"
#include "../Areas/Room.h"
#include "../GameData.h"
#include "../Ships/Ship.h"

void PlayingState::processInput(const std::string &input, std::shared_ptr<Connection> connection) {
    std::string input_string = input;
    std::string command = Tokenizer::GetFirstToken(input_string);
    command = Tokenizer::LowerCase(command);
    if (m_cmd_map.find(command) != m_cmd_map.end()) {
        m_cmd_map[command](input_string, connection, game_data);
        connection->SetPrompt(GetPrompt(connection));
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
    connection->SetPrompt(GetPrompt(connection));
    connection->SetLoggedIn(true);
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
    auto player = connection->GetPlayer();
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

void PlayingState::CmdHelp(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data) {
    Sender::Send("'chat' to chat\n'tell <character>' to direct tell\n'online' to see who is online\n", connection);
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
    if(room->GetItems().size() > 0) {
        ss << Format::MAGENTA << "Items:" << Format::NL;
        for (auto item : room->GetItems()) {
            ss << item.first << Format::NL;
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
        if (connection->GetPlayer()->IsVisible()) {
            std::stringstream departed_ss;
            std::stringstream arrived_ss;
            std::string player_name = player->GetPlayerName();
            departed_ss << player_name << " has left " << depart_string << Format::NL;
            arrived_ss << player_name << " has arrived from " << arrive_string << Format::NL;
            Sender::SendToMultiple(departed_ss.str(), game_data->GetLoggedInConnections(),
                                   departed_room->GetVisiblePlayers());
            auto new_room = game_data->GetRoom(area->GetID(), new_room_int, false);
            Sender::SendToMultiple(arrived_ss.str(), game_data->GetLoggedInConnections(),
                                   new_room->GetVisiblePlayers(connection->GetID()));
        }
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
        newroom = std::atoi(room_number_string.c_str());
    }

    auto player = connection->GetPlayer();
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
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    bool found = false;
    std::string ship_name = Tokenizer::GetFirstToken(data);
    for (auto ship: room->GetShips()) {
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
                //player->SetShipID(ship.second->GetID());
                player->SetShip(ship.second);
                player->SetInShip(true);
                //player->SetRoomID(0);
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
    ss << "Inventory:" << Format::NL;
    for(auto item : player->GetItems()) {
        ss << item.first << Format::NL;
    }
    Sender::Send(ss.str(), connection);
}

void PlayingState::CmdGet(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data) {
    std::string input_data = input;
    std::string item_name = Tokenizer::GetFirstToken(input_data);
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    auto items = room->GetItems();
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
    std::string item_name = Tokenizer::GetFirstToken(input_data);
    auto player = connection->GetPlayer();
    auto room = player->GetRoom();
    auto items = player->GetItems();
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

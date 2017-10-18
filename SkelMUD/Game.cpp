#include "Game.h"
#include "File.h"
#include "States/PlayingState.h"
#include "States/LoginState.h"
#include "Sender.h"
#include "Format.h"
#include "States/CreateCharacterState.h"
#include "States/BuildingState.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <thread>

#ifndef _WIN32
#include <sstream>
#include <unistd.h>
#include <mutex>

#endif

Game::Game() {
    isRunning = true;
    Logger::Debug("Setting up GameData");
    m_game_data = std::make_shared<GameData>();
    connection_id = 0;
    elapsed = std::time(nullptr);
    Logger::Debug("Initializing States");
    initStates();
}

std::mutex Game::game_mutex;

Game::~Game() {

}

void Game::Start() {
    std::thread listener(&Game::listenerThread, this);
    listener.detach();
    std::list<int> disconnected;
    while(isRunning) {
        std::map<int, std::shared_ptr<Connection>> connection_map = m_game_data->GetAllConnections();
        for(auto connection_map_entry : connection_map)
        {
            std::lock_guard<std::mutex> lock(Game::game_mutex);
            std::shared_ptr<Connection> connection = connection_map_entry.second;
            if(std::time(nullptr) - elapsed >= 1) {
                std::shared_ptr<GameState> state = state_map[connection->GetState()];
                connection->SetPrompt(state->GetPrompt(connection));
            }
            if(!connection->IsConnected())
            {
                if(connection->IsLoggedIn()) {
                    auto player = m_game_data->GetPlayer(connection->GetCharacterName());
                    m_game_data->GetRoom(player->GetLocationID(),
                                         player->GetRoomID(),
                                         player->IsInShip())->RemovePlayer(player->GetID());
                }
                m_game_data->EraseConnection(connection->GetID());
                std::stringstream ss;
                ss << connection->GetIP() << " has disconnected (connection dropped)";
                Logger::Info(ss.str());
                continue;
            }
            if(connection->IsPromptTick()) {
                Sender::UpdatePrompt(connection);
                //Sender::Send(state->GetPrompt(connection), connection);
//                Sender::Send(Format::SAVE + Format::UP + Format::FRONT_LINE + state->GetPrompt(connection) + Format::RESTORE,
//                             connection);
            }
            std::string received = connection->GetNextReceived();
            Utils::RemoveEndline(received);
            connection->FlushOutput();
            if(received == "")
                continue;
            std::stringstream ss;
            ss << "Received data (" << connection->GetIP() << "): " << received;
            Logger::Debug(ss.str());
            state_map[connection->GetState()]->processInput(received, connection);
            if(connection->IsStateChanged())
            {
                state_map[connection->GetState()]->init(connection);
                connection->ResetStateChanged();
            }

            // Sender::Send("\r\n", connection);
            // Sender::Send(state_map[connection->GetState()]->GetPrompt(connection), connection);
        }
        if(std::time(nullptr) - elapsed >= 1) {
            ProcessShips();
            elapsed = std::time(nullptr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::ProcessShips() {
    for(auto ship : m_game_data->GetShips().GetShips()) {
        if(!ship.second->IsInOrbit()) {
            Utils::Vector3 coords = ship.second->GetCoordinates();
            Utils::Vector3 velocity = ship.second->GetVelocity();
            double x = coords.x + velocity.x;
            double y = coords.y + velocity.y;
            double z = coords.z + velocity.z;
            ship.second->SetCoordinates(x, y, z);
            bool is_moving = (velocity.x + velocity.y + velocity.z) != 0;
            double speed = Utils::GetDistance(0, 0, 0, velocity);
            if(ship.second->IsInSpace() and
               is_moving and
               Utils::GetDistance(ship.second->GetCoordinates(),
                                  ship.second->GetDestination()) <= speed * 1.3) {
                Sender::SendToMultiple("Arrived at destination\n",
                                       m_game_data->GetLoggedInConnections(),
                                       ship.second->GetPlayerIDs());
                ship.second->SetVelocity(0, 0, 0);
                //ship.second->SetCoordinates(ship.second->GetDestination());
            }
        }
    }
}

void Game::listenerThread() {
    ServerSocket socket(m_game_data->GetConfiguration().GetPort());
    socket.Listen();
    while(isRunning) {
        DataSocket dataSocket = socket.Accept();
        auto connection = std::make_shared<Connection>(dataSocket);
        connection->SetState(USERNAME);
        state_map[USERNAME]->init(connection);
        connection->ResetStateChanged();
        // Sender::Send(connection->GetPrompt(), connection);
        std::lock_guard<std::mutex> guard(Game::game_mutex);
        // TODO: connection_id temporary for debugging
        // TODO: eventually connection_id could increment beyond the size of int
        connection->SetID(connection->GetSocket());
        ++connection_id;
        m_game_data->AddConnection(connection);
        connection->Run();
    }
}

void Game::initStates() {
    state_map[USERNAME] = std::make_shared<LoginState>(m_game_data);
    state_map[PLAYING] = std::make_shared<PlayingState>(m_game_data);
    state_map[CHARACTERCREATION] = std::make_shared<CreateCharacterState>(m_game_data);
    state_map[BUILDING] = std::make_shared<BuildingState>(m_game_data);
}

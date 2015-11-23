#include "Game.h"
#include "File.h"
#include "States/LoginState.h"
#include "States/PlayingState.h"
#include <iostream>
#include <algorithm>
#include <thread>

#ifndef _WIN32
#include <sstream>
#include <unistd.h>
#include <mutex>

#endif

Game::Game() {
//    // List of possible directional commands. Used to determine if entered command is a direction
//    DIRECTIONS = {"NORTH", "N", "SOUTH", "S", "EAST", "E", "WEST", "W", "NORTHEAST", "NE", "NORTHWEST",
//                  "NW", "SOUTHEAST", "SE", "SOUTHWEST", "SW", "UP", "U", "DOWN", "D"};
    // Presort the list for later use of binary_search
    //std::sort(DIRECTIONS.begin(), DIRECTIONS.end());
    isRunning = true;
    port = 4321;
    data = std::make_shared<GameData>();
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
        std::map<int, std::shared_ptr<Connection>> connection_map = data->GetAllConnections();
        for(auto connection_map_entry : connection_map)
        {
            std::lock_guard<std::mutex> lock(Game::game_mutex);
            std::shared_ptr<Connection> connection = connection_map_entry.second;
            if(!connection->IsConnected())
            {
                disconnected.push_back(connection->GetSocket());
                std::stringstream ss;
                ss << connection->GetIP() << " has disconnected (connection dropped)";
                Logger::Info(ss.str());
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
            // connection->FlushOutput();
        }
        for(auto disconnect_entry : disconnected)
        {
            data->EraseConnection(disconnect_entry);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::listenerThread() {
    ServerSocket socket(port);
    socket.Listen();
    while(isRunning) {
        DataSocket dataSocket = socket.Accept();
        auto connection = std::make_shared<Connection>(dataSocket);
        std::lock_guard<std::mutex> guard(Game::game_mutex);
        connection->SetState(LOGIN);
        state_map[LOGIN]->init(connection);
        data->AddConnection(connection);
        connection->Run();
    }
}

void Game::initStates() {
    state_map[LOGIN] = std::make_shared<LoginState>(data);
    state_map[PLAYING] = std::make_shared<PlayingState>(data);
}

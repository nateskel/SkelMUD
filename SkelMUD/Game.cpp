#include "Game.h"
#include "Tokenizer.h"
#include "File.h"
#include "States/LoginState.h"
#include "States/TestState.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <thread>
#include <chrono>

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
    data = GameData();
    state = std::make_shared<LoginState>(data);
}

std::mutex Game::game_mutex;

Game::~Game() {

}

void Game::Start() {
    std::thread listener(&Game::listenerThread, this);
    listener.detach();
    while(isRunning) {
        std::map<int, std::shared_ptr<Connection>> connection_map = data.GetAllConnections();
        for(auto connection_map_entry : connection_map)
        {
            std::lock_guard<std::mutex> lock(Game::game_mutex);
            std::shared_ptr<Connection> connection = connection_map_entry.second;
            //Connection connection = connection_map_entry.second;
            connection->FlushOutput();
            std::string received = connection->GetNextReceived();
            if(received == "")
                continue;
            else if(Tokenizer::GetFirstToken(received, false) == "next")
                state = std::make_shared<TestState>(data);
            state->processInput(received, connection);
            connection->FlushOutput();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::listenerThread() {
    ServerSocket socket(port);
    socket.Listen();
    while(isRunning) {
        DataSocket dataSocket = socket.Accept();
        //std::shared_ptr<Connection> connection = std::make_shared<Connection>(dataSocket);
        //m_connection_map[connection->GetSocket()] = connection;
        auto connection = std::make_shared<Connection>(dataSocket);
        std::lock_guard<std::mutex> guard(Game::game_mutex);
        data.AddConnection(connection);
        connection->Run();
    }
}

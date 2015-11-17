#include "Game.h"
#include "Tokenizer.h"
#include "File.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <thread>
#include <chrono>

#ifndef _WIN32
#include <sstream>
#include <unistd.h>

#endif

Game::Game() {
    // List of possible directional commands. Used to determine if entered command is a direction
    DIRECTIONS = {"NORTH", "N", "SOUTH", "S", "EAST", "E", "WEST", "W", "NORTHEAST", "NE", "NORTHWEST",
                  "NW", "SOUTHEAST", "SE", "SOUTHWEST", "SW", "UP", "U", "DOWN", "D"};
    // Presort the list for later use of binary_search
    std::sort(DIRECTIONS.begin(), DIRECTIONS.end());
    isRunning = true;
    port = 4321;

}

Game::~Game() {

}

void Game::Start() {
    std::thread listener(&Game::listenerThread, this);
    listener.detach();
    while(isRunning) {
        for(auto connection_map : m_connection_map)
        {
            std::shared_ptr<Connection> connection = connection_map.second;
            connection->FlushOutput();
            std::string received = connection->GetNextReceived();
            if(Tokenizer::GetFirstToken(received) == "quit")
            {
                connection->Close();
                isRunning = false;
            }
            else {
                connection->AddOutput(received);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::listenerThread() {
    ServerSocket socket(port);
    socket.Listen();
    while(isRunning) {
        DataSocket dataSocket = socket.Accept();
        std::shared_ptr<Connection> connection = std::make_shared<Connection>(dataSocket);
        m_connection_map[connection->GetSocket()] = connection;
        connection->Run();
    }
}

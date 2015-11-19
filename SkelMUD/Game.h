#ifndef GAME_H
#define GAME_H

#include "Connection.h"
#include "Player.h"
#include "Sender.h"
#include "TextProcessor.h"
#include "Planet.h"
#include "Utils.h"
#include "Logger.h"
#include "States/GameState.h"
#include "GameData.h"
#include <map>
#include <mutex>

class Game {
public:
    Game();
    ~Game();

    void Start();

private:
    GameData data;
    std::shared_ptr<GameState> state;
    // std::vector<std::string> DIRECTIONS;
    //std::map<SOCKET, std::shared_ptr<Connection>> m_connection_map;
    bool isRunning;
    int port;
    static std::mutex game_mutex;
    void listenerThread();
};

#endif // GAME_H

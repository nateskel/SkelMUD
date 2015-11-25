#ifndef GAME_H
#define GAME_H

#include "Connection.h"
#include "Player.h"
#include "TextProcessor.h"
#include "Planet.h"
#include "Utils.h"
#include "Logger.h"
#include "States/GameState.h"
#include "GameData.h"
#include <map>
#include <mutex>

#define LOGIN "Login"
#define PLAYING "Playing"

class Game {
public:
    Game();
    ~Game();

    void Start();

private:
    std::shared_ptr<GameData> data;
    std::map<std::string, std::shared_ptr<GameState>> state_map;
    bool isRunning;
    int port;
    int connection_id;
    static std::mutex game_mutex;
    void listenerThread();
    void initStates();
};

#endif // GAME_H

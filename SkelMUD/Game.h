#ifndef GAME_H
#define GAME_H

#include "Connection.h"
#include "Player.h"
#include "Sender.h"
#include "TextProcessor.h"
#include "Planet.h"
#include "Thread.h"
#include "Utils.h"
#include "Logger.h"
#include <map>

class Game {
public:
    Game();

    ~Game();

    void Start();

private:
    std::vector<std::string> DIRECTIONS;
    std::map<SOCKET, std::shared_ptr<Connection>> m_connection_map;
    bool isRunning;
    int port;
    void listenerThread();
};

#endif // GAME_H

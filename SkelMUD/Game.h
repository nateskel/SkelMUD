#ifndef GAME_H
#define GAME_H

#include "Connection.h"
#include "Player.h"
#include "TextProcessor.h"
#include "Planets/Planet.h"
#include "Utils.h"
#include "Logger.h"
#include "States/GameState.h"
#include "GameData.h"
#include "Items/Wieldable.h"
#include "Skills/Skill.h"
#include <map>
#include <mutex>

//#define LOGIN "Login"
//#define USERNAME "Username"
//#define PLAYING "Playing"

class Game {
public:
    Game();
    ~Game();
    void Start();
    void Start(int port);

    const std::string LOGIN = "Login";
    const std::string USERNAME = "Username";
    const std::string PLAYING = "Playing";
    const std::string CHARACTERCREATION = "CharacterCreation";
    const std::string BUILDING = "Building";

private:
    std::shared_ptr<GameData> m_game_data;
//    std::map<std::string, std::shared_ptr<GameState>> state_map;
    bool isRunning;
    int port;
    int connection_id;
    double main_elapsed;
    double regen_elapsed;
    static std::mutex game_mutex;
    void listenerThread();
//    void initStates();
    void ProcessShips();
    void ProcessCombat();
    void ProcessSkill(std::shared_ptr<Skill> skill);
    void ProcessRegen();
    static float GetScaledModifier(const std::shared_ptr<Player> &player, ScaleAttribute scaled);
};

#endif // GAME_H

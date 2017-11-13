//
// Created by nate on 11/17/15.
//

#ifndef SKELMUD_GAMESTATE_H
#define SKELMUD_GAMESTATE_H


#include <map>
#include <memory>

class Connection;

class GameData;

class GameState {
public:
    GameState() {};
    GameState(std::shared_ptr<GameData> data);// : game_data(data) {};
    virtual void processInput(const std::string& input, std::shared_ptr<Connection> connection) = 0;
    virtual void init(std::shared_ptr<Connection> connection);
    virtual void Shutdown(std::shared_ptr<Connection> connection) = 0;
    virtual std::string GetPrompt(std::shared_ptr<Connection> connection);
protected:
    std::shared_ptr<GameData> game_data;
};

#endif //SKELMUD_GAMESTATE_H

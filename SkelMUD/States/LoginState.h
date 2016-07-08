//
// Created by skelton on 3/17/16.
//

#ifndef SKELMUD_USERNAMESTATE_H
#define SKELMUD_USERNAMESTATE_H

#include "GameState.h"

class LoginState : public GameState {

private:
    void processUsername(const std::string &input, std::shared_ptr<Connection> connection);
    void processPassword(const std::string &input, std::shared_ptr<Connection> connection);
    void processNewPassword(const std::string &input, std::shared_ptr<Connection> connection);
    void processNewAccount(const std::string &input, std::shared_ptr<Connection> connection);
    void reset(std::shared_ptr<Connection> connection);
    std::map<int, int> m_state_map;
    enum states {
        USERNAME,
        PASSWORD,
        NEWACCOUNT,
        NEWPASSWORD,
    };
public:
    LoginState(std::shared_ptr<GameData> data) : GameState(data) { }
    virtual void processInput(const std::string& input, std::shared_ptr<Connection> connection) override;
    virtual void init(std::shared_ptr<Connection> connection) override;
    void RemoveConnection(const int& connection_id);
};


#endif //SKELMUD_USERNAMESTATE_H

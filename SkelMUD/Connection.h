//
// Created by nate on 11/13/15.
//

#ifndef SKELMUD_CONNECTION_H
#define SKELMUD_CONNECTION_H

#include <string>
#include <queue>
#include "Network.h"
#include "Accounts/Account.h"
#include "States/GameState.h"

class Player;

class GameState;

enum GameStates : short;

class Connection {
public:
    Connection(DataSocket &dataSocket) : dataSocket(dataSocket), id(-1) { }

private:
    void connectionThread();
    DataSocket dataSocket;
    bool is_connected;
    bool state_changed;
    bool m_advanced_prompt;
    int id;
    std::string m_send_buffer;
    std::shared_ptr<GameState> m_state;
    std::queue<std::string> m_receive_buffer;
    std::string owner_ip;
    Account m_account;
    std::string m_username;
    std::string m_password;
    std::string m_character_race;
    std::string m_character_class;
    std::string m_character_name;
    int prompt_tick;
    std::string m_prompt;
    bool m_loggedin;
    std::shared_ptr<Player> m_player;
    int offset;
    bool m_is_building;

public:
    Connection();
    void Run();
    void Send(char* output);
    void AddOutput(const std::string &output);
    void FlushOutput();
    void UpdatePrompt();
    void Close();
    void SetState(const GameStates& connection_state, const std::shared_ptr<GameData> &game_data);
    std::shared_ptr<GameState> GetState();
    SOCKET GetSocket();
    std::string GetNextReceived();
    std::string GetIP();
    bool IsConnected() const;
    bool IsStateChanged() const;
    void ResetStateChanged();
    int GetID() const;
    void SetID(int connection_id);
    Account GetAccount();
    std::string GetUsername();

    void SetAccount(const Account &account);
    void SetUsername(const std::string &username);

    std::string GetPrompt();
    bool IsPromptTick();
    void SetLoggedIn(bool logged);
    bool IsLoggedIn() const;
    const std::string &GetCharacterRace() const;
    void SetCharacterRace(const std::string& character_race);
    const std::string &GetCharacterClass() const;
    void SetCharacterClass(const std::string& m_character_class);
    const std::string &GetCharacterName() const;
    void SetCharacterName(const std::string& character_name);
    void SetPlayer(const std::shared_ptr<Player>& player);
    std::shared_ptr<Player> GetPlayer();
    void AdvancedPrompt(bool state);
    void SetBuilding(bool building);
    bool IsBuilding() const;
};

#endif //SKELMUD_CONNECTION_H

//
// Created by nate on 11/13/15.
//

#include <thread>
#include <vector>
#include <list>
#include <sstream>
#include "Connection.h"
#include "Logger.h"
#include "Tokenizer.h"
#include "Sender.h"
#include "Format.h"
#include "Player.h"
#include "States/GameState.h"
#include "States/StateFactory.h"

#define MAX_TICK 100

void Connection::Run() {
    std::thread connThread(&Connection::connectionThread, this);
    is_connected = true;
    owner_ip = dataSocket.GetIP();
    std::stringstream ss;
    ss << "Incoming connection from " << owner_ip;
    Logger::Debug(ss.str());
    connThread.detach();
    health = 100;
    prompt_tick = 0;
    logged_in = false;
}

void Connection::connectionThread() {
    char buffer[1000];
    while(is_connected) {
        memset(buffer, 0, sizeof(buffer));
        int result = dataSocket.Receive(buffer);
        if(result <= 0) {
            is_connected = false;
        }
        else {
            auto commands = Tokenizer::GetAllTokens(buffer, '\n');
            for(auto command: commands) {
                std::string string_data = std::string(command);
                m_receive_buffer.push_back(string_data);
            }
        }
    }
    Close();
}

void Connection::Send(char* output) {
    dataSocket.Send(output);
}

void Connection::AddOutput(std::string output) {
    m_send_buffer.append(output);
}

void Connection::FlushOutput() {
    if (m_send_buffer == "")
        return;
    UpdatePrompt();
}

void Connection::UpdatePrompt() {
    long size = Tokenizer::GetAllTokens(GetState()->GetLastPrompt(), '\n').size();
    m_state->CleanPrompt(*this);
    prompt_tick = 0;
    auto prompt = GetState()->GetPrompt(*this);
    for(int i = 0; i < size; ++i) {
        m_send_buffer.insert(0, Format::UP + Format::ERASE);
    }
    m_send_buffer.insert(0, Format::SAVE + Format::FRONT_LINE);
    m_send_buffer.append(prompt + Format::RESTORE + Format::RESET);
    std::vector<char> output(m_send_buffer.begin(), m_send_buffer.end());
    output.push_back('\0');
    int sent = dataSocket.Send(&output[0]);
    if (sent == -1) {
        is_connected = false;
    }
    m_send_buffer.clear();
}

void Connection::Close() {
    dataSocket.Close();
}

std::string Connection::GetNextReceived() {
    if(m_receive_buffer.size() == 0)
        return "";
    std::string output = m_receive_buffer.front();
    m_receive_buffer.pop_front();
    return output;
}

SOCKET Connection::GetSocket() {
    return dataSocket.GetSocket();
}

Connection::Connection() {
    m_state = nullptr;
    character_class = "";
    character_race = "";
}

void Connection::SetState(const GameStates &connection_state, std::shared_ptr<GameData> game_data) {
    m_state = StateFactory::GetGameState(connection_state, game_data);
    state_changed = true;
}

std::shared_ptr<GameState> Connection::GetState() {
    return m_state;
}

std::string Connection::GetIP() {
    return owner_ip;
}

bool Connection::IsConnected() {
    return is_connected;
}

int Connection::GetID() {
    return id;
}

void Connection::SetID(int connection_id) {
    id = connection_id;
}

std::string Connection::GetUsername() {
    return username;
}

std::string Connection::GetPassword() {
    return password;
}

void Connection::SetAccount(Account account) {
    this->account = account;
}

void Connection::SetUsername(std::string username) {
    this->username = username;
}

void Connection::SetPassword(std::string password) {
    this->password = password;
}

Account Connection::GetAccount() {
    return account;
}

bool Connection::IsStateChanged() {
    return state_changed;
}

void Connection::ResetStateChanged() {
    state_changed = false;
}

std::string Connection::GetPrompt() {
    return "";
    //return m_prompt;
}

bool Connection::IsPromptTick() {
    if(!m_state->IsDirty(*this))
        return false;
    if(prompt_tick > MAX_TICK)
    {
        prompt_tick = 0;
        m_state->CleanPrompt(*this);
        return true;
    }
    else
    {
        prompt_tick++;
        return false;
    }
}

void Connection::SetLoggedIn(bool logged) {
    logged_in = logged;
}

bool Connection::IsLoggedIn() {
    return logged_in;
}

void Connection::SetPrompt(std::string prompt) {
    if (m_prompt != prompt) {
        m_prompt = prompt;
        m_dirty_prompt = true;
    }
}

int Connection::GetHealth() {
    return GetPlayer()->GetHP();
}

void Connection::TickNow() {
    m_dirty_prompt = true;
    prompt_tick = MAX_TICK + 1;
}

const std::string &Connection::GetCharacterRace() const {
    return character_race;
}

void Connection::SetCharacterRace(const std::string &character_race) {
    Connection::character_race = character_race;
}

const std::string &Connection::GetCharacterClass() const {
    return character_class;
}

void Connection::SetCharacterClass(const std::string &character_class) {
    Connection::character_class = character_class;
}

const std::string &Connection::GetCharacterName() const {
    return character_name;
}

void Connection::SetCharacterName(const std::string &character_name) {
    Connection::character_name = character_name;
}

void Connection::SetPlayer(std::shared_ptr<Player> player) {
    m_player = player;
}

std::shared_ptr<Player> Connection::GetPlayer() {
    return m_player;
}
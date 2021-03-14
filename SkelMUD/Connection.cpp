//
// Created by nate on 11/13/15.
//

#include <thread>
#include <vector>
#include <sstream>
#include "Connection.h"
#include "Logger.h"
#include "Tokenizer.h"
#include "Format.h"
#include "Player.h"
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
    prompt_tick = 0;
    m_loggedin = false;
    offset = 0;
    m_advanced_prompt = false;
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
            for(const auto &command: commands) {
                std::string string_data = std::string(command);
                m_receive_buffer.push(string_data);
            }
        }
    }
    Close();
}

void Connection::Send(char* output) {
    dataSocket.Send(output);
}

void Connection::AddOutput(const std::string &output) {
    m_send_buffer.append(output);
}

void Connection::FlushOutput() {
    if (m_send_buffer.empty())
        return;
    offset += 1;
    UpdatePrompt();
}

void Connection::UpdatePrompt() {
    long size = 0;
    size = Tokenizer::CountLines(GetState()->GetLastPrompt());
    m_state->CleanPrompt(*this);
    prompt_tick = 0;
    auto prompt = GetState()->GetPrompt(*this);
    if(m_advanced_prompt) {
        for (int i = 0; i < size; ++i) {
            m_send_buffer.insert(0, Format::UP + Format::ERASE);
        }
        m_send_buffer.insert(0, Format::SAVE + Format::FRONT_LINE);
        m_send_buffer.append(prompt + Format::RESTORE + Format::RESET);
    }
    else {
        m_send_buffer.append(prompt + Format::RESET);
    }
    //offset = Tokenizer::CountLines(m_send_buffer);
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
    if(m_receive_buffer.empty())
        return "";
    std::string output = m_receive_buffer.front();
    m_receive_buffer.pop();
    return output;
}

SOCKET Connection::GetSocket() {
    return dataSocket.GetSocket();
}

Connection::Connection() {
    m_state = nullptr;
    m_character_class = "";
    m_character_race = "";
    m_is_building = false;
    is_connected = false;
    state_changed = false;
    m_advanced_prompt = false;
    id = 0;
    prompt_tick = 0;
    m_loggedin = false;
    offset = 0;
}

void Connection::SetState(const GameStates &connection_state, const std::shared_ptr<GameData> &game_data) {
    m_state = StateFactory::GetGameState(connection_state, game_data);
    state_changed = true;
}

std::shared_ptr<GameState> Connection::GetState() {
    return m_state;
}

std::string Connection::GetIP() {
    return owner_ip;
}

bool Connection::IsConnected() const {
    return is_connected;
}

int Connection::GetID() const {
    return id;
}

void Connection::SetID(int connection_id) {
    id = connection_id;
}

std::string Connection::GetUsername() {
    return m_username;
}

void Connection::SetAccount(const Account &account) {
    this->m_account = account;
}

void Connection::SetUsername(const std::string &username) {
    this->m_username = username;
}

Account Connection::GetAccount() {
    return m_account;
}

bool Connection::IsStateChanged() const {
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
    m_loggedin = logged;
}

bool Connection::IsLoggedIn() const {
    return m_loggedin;
}

const std::string &Connection::GetCharacterRace() const {
    return m_character_race;
}

void Connection::SetCharacterRace(const std::string &character_race) {
    m_character_race = character_race;
}

const std::string &Connection::GetCharacterClass() const {
    return m_character_class;
}

void Connection::SetCharacterClass(const std::string &character_class) {
    m_character_class = character_class;
}

const std::string &Connection::GetCharacterName() const {
    return m_character_name;
}

void Connection::SetCharacterName(const std::string& character_name) {
    Connection::m_character_name = character_name;
}

void Connection::SetPlayer(const std::shared_ptr<Player>& player) {
    m_player = player;
}

std::shared_ptr<Player> Connection::GetPlayer() {
    return m_player;
}

void Connection::AdvancedPrompt(bool state) {
    m_advanced_prompt = state;
}

void Connection::SetBuilding(bool building) {
    m_is_building = building;
}

bool Connection::IsBuilding() const {
    return m_is_building;
}

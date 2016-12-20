#include "Player.h"
#include "Logger.h"

Player::Player() : Entity() {
    m_room_id = 1;
    m_planet_id = 0;
    m_visible = true;
}

Player::Player(int player_id, std::string player_name) {
    m_room_id = 1;
    m_planet_id = 0;
    m_player_id = player_id;
    m_name = player_name;
    m_visible = true;
}

Player::Player(int player_id, std::string player_name, std::string player_class, std::string player_race) : Entity() {
    m_player_id = player_id;
    m_name = player_name;
    m_class = player_class;
    m_race = player_race;
    m_room_id = 1;
    m_planet_id = 0;
    m_visible = true;
}

int Player::GetID() {
    return m_player_id;
}

void Player::SetID(int id) {
    m_player_id = id;
}

const std::string &Player::GetPlayerName() const {
    return m_name;
}

void Player::SetPlayerName(const std::string &name) {
    m_name = name;
}

const std::string &Player::GetPlayerRace() const {
    return m_race;
}

void Player::SetPlayerRace(const std::string &race) {
    m_race = race;
}

const std::string &Player::GetPlayerClass() const {
    return m_class;
}

void Player::SetPlayerClass(const std::string &character_class) {
    m_class = character_class;
}

bool Player::IsVisible() {
    return m_visible;
}

void Player::SetVisible(const bool visible) {
    m_visible = visible;
}
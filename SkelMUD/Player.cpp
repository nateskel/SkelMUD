#include "Player.h"
#include "Logger.h"

Player::Player() : Entity() {
    m_room_id = 0;
    m_planet_id = 0;
}

Player::Player(int player_id, std::string player_name, std::string player_class, std::string player_race) : Entity() {
    Logger::Debug(player_name);
    m_player_id = player_id;
    m_name = player_name;
    m_class = player_class;
    m_race = player_race;
    m_room_id = 0;
    m_planet_id = 0;
}

int Player::GetID() {
    return m_player_id;
}
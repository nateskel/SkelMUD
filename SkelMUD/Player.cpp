#include "Player.h"

Player::Player() : Entity() {
    m_room_id = 0;
    m_planet_id = 0;
}

Player::Player(int player_id, std::string player_name) : Entity() {
    m_player_id = player_id;
    m_name = player_name;
    m_room_id = 0;
    m_planet_id = 0;
}

Player::~Player() {

}

int Player::GetID() {
    return m_player_id;
}
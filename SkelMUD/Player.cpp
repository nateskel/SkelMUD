#include "Player.h"

Player::Player()
{
	m_room_id = 0;
	m_planet_id = 0;
}

Player::Player(int player_id, std::string player_name)
{
	m_player_id = player_id;
	m_name = player_name;
	SetRoomID(0);
	SetPlanetID(0);
}

Player::~Player()
{

}

int Player::GetID()
{
	return m_player_id;
}
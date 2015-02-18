#include "Player.h"

Player::Player()
{
	m_room_id = 0;
	m_planet_id = 0;
}

Player::Player(int player_id, std::string player_name)
{
	m_player_id = player_id;
	m_player_name = player_name;
	SetRoom(0);
	SetPlanet(0);
}

Player::~Player()
{

}

void Player::SetName(std::string name)
{
	m_player_name = name;
}

std::string Player::GetName()
{
	return m_player_name;
}

int Player::GetID()
{
	return m_player_id;
}
/*
void Player::SetRoom(int id)
{
	m_room_id = id;
}

int Player::GetRoom()
{
	return m_room_id;
}

void Player::SetPlanet(int id)
{
	m_planet_id = id;
}

int Player::GetPlanet()
{
	return m_planet_id;
}
*/
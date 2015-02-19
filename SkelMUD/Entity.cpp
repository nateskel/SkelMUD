#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::SetPlanetID(int planet_id)
{
	m_planet_id = planet_id;
}

int Entity::GetPlanetID()
{
	return m_planet_id;
}

void Entity::SetRoomID(int room_id)
{
	m_room_id = room_id;
}

int Entity::GetRoomID()
{
	return m_room_id;
}

void Entity::SetName(std::string name)
{
	m_name = name;
}

std::string Entity::GetName()
{
	return m_name;
}


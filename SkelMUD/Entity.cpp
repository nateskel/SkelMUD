#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::SetPlanet(int planet_id)
{
	m_planet_id = planet_id;
}

int Entity::GetPlanet()
{
	return m_planet_id;
}

void Entity::SetRoom(int room_id)
{
	m_room_id = room_id;
}

int Entity::GetRoom()
{
	return m_room_id;
}

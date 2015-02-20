#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity
{
public:
	Entity();
	~Entity();
	void SetName(std::string name);
	std::string GetName();
	void SetPlanetID(int planet_id);
	int GetPlanetID();
	void SetRoomID(int room_id);
	int GetRoomID();
protected:
	std::string m_name;
	int m_planet_id;
	int m_room_id;
private:
};

#endif // ENTITY_H
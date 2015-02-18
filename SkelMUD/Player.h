#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>

class Player: public Entity
{
public:
	Player();
	Player(int player_id, std::string player_name);
	~Player();
	void SetName(std::string name);
	std::string GetName();
	int GetID();
	//void SetRoom(int id);
	//int GetRoom();
	//void SetPlanet(int id);
	//int GetPlanet();
protected:
	int m_player_id;
	std::string m_player_name;
	//int m_planet_id;
	//int m_room_id;
	
private:
};
#endif // PLAYER_H

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

protected:
	int m_player_id;
	std::string m_player_name;
	
private:
};
#endif // PLAYER_H

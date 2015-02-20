#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity
{
public:
	Entity(int max_hp, int max_sp, int max_stamina);
	Entity();
	~Entity();
	void SetName(std::string name);
	std::string GetName();
	void SetPlanetID(int planet_id);
	int GetPlanetID();
	void SetRoomID(int room_id);
	int GetRoomID();
	void SetHP(int hp);
	void SetSP(int sp);
	void SetStamina(int stamina);
	void SetMaxHP(int maxhp);
	void SetMaxSP(int maxsp);
	void SetMaxStamina(int maxstamina);
	int GetHP();
	int GetSP();
	int GetStamina();
	int GetMaxHP();
	int GetMaxSP();
	int GetMaxStamina();

protected:
	std::string m_name;
	int m_planet_id;
	int m_room_id;
	int m_max_hp;
	int m_hp;
	int m_max_sp;
	int m_sp;
	int m_max_stamina;
	int m_stamina;
private:
};

#endif // ENTITY_H
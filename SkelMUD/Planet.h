#ifndef PLANET_H
#define PLANET_H

#include "Player.h"
#include "Planet.h"
#include <map>
#include <vector>

class Room;

class Planet
{
public:
	Planet();
	~Planet();
	void AddRoom(Room* room);
	Room* GetRoom(int id);
	bool MoveNorth(int room_id, int player_id);
	bool MoveSouth(int room_id, int player_id);
	bool MoveEast(int room_id, int player_id);
	bool MoveWest(int room_id, int player_id);
	void SetName(std::string name);
	struct Coordinates
	{
		int x;
		int y;
		int z;
	};
	Coordinates coordinates;

protected:
private:
	std::string m_planet_name;
	std::vector<Room*> m_rooms;
	void ChangeRoom(int old_room, int new_room, int player_id);
};

class Room
{
public:
	Room();
	Room(std::string long_desc,
		std::string short_desc,
		int n,
		int s,
		int e,
		int w,
		int ne,
		int nw,
		int se,
		int sw,
		int u,
		int d);
	~Room();
	void RemovePlayer(int id);
	void AddPlayer(Player* player);
	Player* GetPlayer(int id);
	std::vector<int> GetPlayers();
	std::vector<int> GetPlayerIDs(int exclude);
	std::string GetLongDescription();
	std::string GetShortDescription();
	int GetNorth();
	int GetSouth();
	int GetEast();
	int GetWest();
protected:
private:
	int m_id;
	std::string m_long_description;
	std::string m_short_description;
	int m_north;
	int m_south;
	int m_east;
	int m_west;
	int m_northwest;
	int m_northeast;
	int m_southwest;
	int m_southeast;
	int m_up;
	int m_down;
	std::map<int, Player*> m_player_map;
};

Planet* BuildPlanet();
#endif // PLANET_H

#ifndef PLANET_H
#define PLANET_H

#include "../Player.h"
#include "Planet.h"
#include <map>
#include <memory>
#include <vector>

class Room;

class Planet {
public:
    Planet() { };
    Planet(std::string name, int id, int x, int y, int z) : m_planet_name(name), m_planet_id(id) {
        coordinates.x = x;
        coordinates.y = y;
        coordinates.z = z;
    };
    ~Planet() { };

    unsigned long AddRoom(std::shared_ptr<Room> room);

    void DeleteRoom(int room_id);

    std::shared_ptr<Room> GetRoom(int id);

    bool MoveNorth(int room_id, int player_id);

    bool MoveSouth(int room_id, int player_id);

    bool MoveEast(int room_id, int player_id);

    bool MoveWest(int room_id, int player_id);

    bool MoveNorthWest(int room_id, int player_id);

    bool MoveNorthEast(int room_id, int player_id);

    bool MoveSouthWest(int room_id, int player_id);

    bool MoveSouthEast(int room_id, int player_id);

    bool MoveUp(int room_id, int player_id);

    bool MoveDown(int room_id, int player_id);

    void SetName(std::string name);

    bool ChangeRoom(int old_room, int new_room, int player_id);

    std::vector<std::shared_ptr<Room>> GetRooms();

    std::string GetName();

    int GetID();

    struct Coordinates {
        int x;
        int y;
        int z;
    };
    Coordinates coordinates;

protected:
private:
    std::string m_planet_name;
    int m_planet_id;
    std::vector<std::shared_ptr<Room>> m_rooms;
};

class Room {
public:
    Room();

    Room(std::string long_desc, std::string short_desc, int n, int s, int e, int w, int ne, int nw, int se,
             int sw, int u, int d);

    ~Room();

    void RemovePlayer(int id);

    void AddPlayer(std::shared_ptr<Player> player);

    std::shared_ptr<Player> GetPlayer(int id);

    std::vector<int> GetVisiblePlayers();

    std::vector<int> GetVisiblePlayers(int exclude);

    std::vector<int> GetPlayers();

    std::vector<int> GetPlayers(int exclude);

    std::vector<std::string> GetVisiblePlayerNames(int exclude);

    std::vector<std::string> GetPlayerNames(int exclude);

    std::string GetLongDescription();

    std::string GetShortDescription();

    int GetNorth();

    int GetSouth();

    int GetEast();

    int GetWest();

    int GetNorthWest();

    int GetNorthEast();

    int GetSouthWest();

    int GetSouthEast();

    int GetUp();

    int GetDown();

    int GetID();

    void SetNorth(int north);

    void SetSouth(int south);

    void SetEast(int east);

    void SetWest(int west);

    void SetNortheast(int ne);

    void SetNorthwest(int nw);

    void SetSoutheast(int se);

    void SetSouthwest(int sw);

    void SetUp(int up);

    void SetDown(int down);

    void SetID(int id);

    void SetLongDesc(std::string long_desc);

    void SetShortDesc(std::string short_desc);

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
    std::map<int, std::shared_ptr<Player>> m_player_map;
};

#endif // PLANET_H

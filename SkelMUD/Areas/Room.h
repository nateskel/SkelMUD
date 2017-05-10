//
// Created by nate on 3/19/17.
//

#ifndef SKELMUD_ROOM_H
#define SKELMUD_ROOM_H

#include <vector>
#include <map>
#include "../Player.h"
//#include "../Ships/Ships.h"
//#include "../Ships/Ship.h"

class Ship;

class Ships;

class Room {
public:
    enum LandingLevel {
        NONE = 0,
        SMALL = 1,
        MEDIUM = 2,
        LARGE = 3,
        CAPITAL = 4,
        SUPER_CAPITAL = 5
    };

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
    LandingLevel m_landing_level;
    std::map<int, std::shared_ptr<Player>> m_player_map;
    std::vector<int> m_ship_ids;
    std::map<int, std::shared_ptr<Ship>> m_ships;

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

    void RemoveShip(int ship_id);

    void AddShipID(int ship_id);

    void AddShipIDs(std::vector<int> ship_ids);

    void PopulateShips(Ships &ships, int area_id);

    std::vector<int> GetShipIDs();

    std::map<int, std::shared_ptr<Ship>> GetShips();

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

    bool IsLandable();

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

    void SetLandingLevel(LandingLevel landing_level);

    LandingLevel GetLandingLevel();

protected:
};


#endif //SKELMUD_ROOM_H

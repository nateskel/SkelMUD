//
// Created by nate on 3/19/17.
//

#ifndef SKELMUD_ROOM_H
#define SKELMUD_ROOM_H

#include <vector>
#include <map>
#include "../Player.h"
#include "../NPCs/NPC.h"

class Area;
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

    enum Directions {
        NORTH = 0,
        SOUTH,
        EAST,
        WEST,
        NORTHWEST,
        NORTHEAST,
        SOUTHWEST,
        SOUTHEAST,
        UP,
        DOWN,
        NUMBER_OF_DIRECTIONS
    };

private:
    int m_id;
    std::string m_long_description;
    std::string m_short_description;
    int m_directions[NUMBER_OF_DIRECTIONS];
    LandingLevel m_landing_level;
    std::map<int, std::shared_ptr<Player>> m_player_map;
    std::map<std::string, std::shared_ptr<NPC>> m_npc_map;
    std::vector<int> m_ship_ids;
    std::map<int, std::shared_ptr<Ship>> m_ships;
    std::map<std::string, int> m_items;
    bool m_is_cockpit;

public:
    Room();

    Room(const std::string &long_desc,
         const std::string &short_desc,
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

    Room(const std::string &long_desc,
         const std::string &short_desc,
         const int *directions);

    ~Room();

    bool IsCockpit() const;

    void SetIsCockpit(bool is_cockpit);

    void RemoveShip(int ship_id);

    void AddShipID(int ship_id);

    void AddShipIDs(std::vector<int> ship_ids);

    void AddShip(const std::shared_ptr<Ship>& ship);

    std::vector<int> GetShipIDs();

    std::map<int, std::shared_ptr<Ship>> GetShips();

    void RemovePlayer(int id);

    void AddPlayer(const std::shared_ptr<Player>& player);

    void AddNPC(const std::shared_ptr<NPC>& npc);

    std::vector<std::string> GetNPCs();

    std::shared_ptr<Player> GetPlayer(int id);

    std::vector<int> GetVisiblePlayers();

    std::vector<int> GetVisiblePlayers(int exclude);

    std::vector<int> GetPlayers();

    std::vector<int> GetPlayers(int exclude);

    std::vector<std::string> GetVisiblePlayerNames(int exclude);

    std::vector<std::string> GetVisiblePlayerNames(const std::string& exclude_name);

    std::vector<std::string> GetPlayerNames(int exclude);

    void AddItem(const std::string&);

    void RemoveItem(const std::string&);

    std::map<std::string, int> GetItems();

    std::string GetLongDescription();

    std::string GetShortDescription();

    int GetDirection(Directions directions);

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

    int GetID() const;

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

    void SetLongDesc(const std::string &long_desc);

    void SetShortDesc(const std::string &short_desc);

    void SetLandingLevel(LandingLevel landing_level);

    LandingLevel GetLandingLevel();

protected:
};


#endif //SKELMUD_ROOM_H

//
// Created by skelton on 12/22/16.
//

#ifndef SKELMUD_AREA_H
#define SKELMUD_AREA_H

#include "../Player.h"
#include "Room.h"
#include <map>
#include <memory>
#include <vector>

class Area {
public:
    Area() { };
    Area(std::string name, int id) : m_area_name(name), m_area_id(id) {}
    ~Area() { };

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

    void PopulateShips(Ships &ships);

protected:
    std::string m_area_name;
    int m_area_id;
    std::vector<std::shared_ptr<Room>> m_rooms;
private:
};

#endif //SKELMUD_AREA_H

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

    void SetName(std::string name);

    bool ChangeRoom(int old_room, int new_room, std::shared_ptr<Player> player);

    std::vector<std::shared_ptr<Room>> GetRooms();

    std::string GetName();

    int GetID();

//    void PopulateShips(Ships &ships);

protected:
    std::string m_area_name;
    int m_area_id;
    std::vector<std::shared_ptr<Room>> m_rooms;
private:
};

#endif //SKELMUD_AREA_H

//
// Created by nate on 3/19/17.
//

#ifndef SKELMUD_SHIPS_H
#define SKELMUD_SHIPS_H

#include "../Ships/Ship.h"

class Ships {

private:
    std::map<int, std::shared_ptr<Ship>> m_ship_map;
public:
    void LoadShips(std::string filename);
    std::shared_ptr<Ship> BuildShip(std::string filename, int id, int health, int shields);
    void AddShip(std::shared_ptr<Ship> ship);
    std::map<int, std::shared_ptr<Ship>> GetShips();
    std::map<int, std::shared_ptr<Ship>> EnumerateShips();
    void SaveShip(int ship_id);
    void SaveShip(Ship ship);
};


#endif //SKELMUD_SHIPS_H

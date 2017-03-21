#ifndef PLANET_H
#define PLANET_H

#include "../Player.h"
#include <map>
#include <memory>
#include <vector>
#include "../Areas/Area.h"

class Room;

class Planet : public Area {
public:
    Planet() : Area(){ };
    Planet(std::string name, int id, int x, int y, int z) : Area(name, id) {
        m_coordinates.x = x;
        m_coordinates.y = y;
        m_coordinates.z = z;
    };

    struct Coordinates {
        int x;
        int y;
        int z;
    } m_coordinates;

    void SetCoordinates(int x, int y, int z);

    Coordinates GetCoordinates();

protected:
private:
};

#endif // PLANET_H

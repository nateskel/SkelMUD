#include "Planet.h"

void Planet::SetCoordinates(int x, int y, int z) {
    m_coordinates.x = x;
    m_coordinates.y = y;
    m_coordinates.z = z;
}

Utils::Coordinates Planet::GetCoordinates() {
    return m_coordinates;
}

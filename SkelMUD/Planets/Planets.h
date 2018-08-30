//
// Created by nate on 10/22/16.
//

#ifndef SKELMUD_PLANETS_H
#define SKELMUD_PLANETS_H

#include <memory>
#include "Planet.h"
#include "../Areas/AreaManager.h"
#include "../Skexml/Node.h"

class Planets {

private:
    std::map<int, std::shared_ptr<Planet>> m_planet_map;
public:
    void LoadPlanets(std::string filename);
    std::shared_ptr<Planet> BuildPlanet(std::string filename, int id, int x, int y, int z);
    void AddPlanet(std::shared_ptr<Planet> planet);
    std::map<int, std::shared_ptr<Planet>> GetPlanets();
    std::map<int, std::shared_ptr<Planet>> EnumeratePlanets();
    void SavePlanet(int planet_id);
    void SavePlanet(Planet planet);
//    void PopulateShips(Ships &ships);
};

#endif //SKELMUD_PLANETS_H

//
// Created by nate on 10/22/16.
//

#include "Planets.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"
#include "../GameData.h"
#include "../Utils.h"
#include "../Tokenizer.h"
#include <algorithm>

void Planets::LoadPlanets(std::string filename) {
    std::shared_ptr<Node> planet_node = SkexmlParser::Parse(filename);
    auto children = planet_node->GetChildren();
    for(auto child : children)
    {
        std::string planet_name = child.first;
        std::string planet_id = child.second->GetAttribute("ID");
        std::string coords = child.second->GetAttribute("Coordinates");
        auto coords_list = Tokenizer::GetAllTokens(coords, ',');
        int id = atoi(planet_id.c_str());
        std::shared_ptr<Planet> planet = BuildPlanet(GameData::PLANET_PATH + planet_name + ".sml", id,
                                                     atoi(coords_list[0].c_str()),
                                                     atoi(coords_list[1].c_str()),
                                                     atoi(coords_list[2].c_str()));

        AddPlanet(planet);
    }
}

std::shared_ptr<Planet> Planets::BuildPlanet(std::string filename, int id, int x, int y, int z) {
    std::shared_ptr<Node> planet_node = SkexmlParser::Parse(filename);
    std::string planet_name = planet_node->GetName();
    std::shared_ptr<Planet> planet = std::make_shared<Planet>(planet_name, id, x, y, z);
    AreaManager::BuildArea(planet_node, planet);
    return planet;
}

void Planets::AddPlanet(std::shared_ptr<Planet> planet) {
    m_planet_map[planet->GetID()] = planet;
}

std::map<int, std::shared_ptr<Planet>> Planets::GetPlanets() {
    return m_planet_map;
}

std::map<int, std::shared_ptr<Planet>> Planets::EnumeratePlanets() {
    auto planets = GetPlanets();
    std::map<int, std::shared_ptr<Planet>> planet_map;
    int count = 0;
    for(auto planet : planets) {
        count++;
        planet_map[count] = planet.second;
    }
    return planet_map;
};

void Planets::SavePlanet(int planet_id) {
    SavePlanet(*m_planet_map[planet_id]);
}

void Planets::SavePlanet(Planet planet) {
    std::string filename = GameData::PLANET_PATH + planet.GetName() + ".sml";
    std::shared_ptr<Node> planet_node = std::make_shared<Node>(planet.GetName());
    AreaManager::BuildRoomNodes(planet, planet_node);
    SkexmlParser::BuildSkeXML(filename, planet_node);
}

//void Planets::PopulateShips(Ships &ships) {
//    for(auto planet: m_planet_map) {
//        planet.second->PopulateShips(ships);
//    }
//}
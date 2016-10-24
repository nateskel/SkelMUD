//
// Created by nate on 10/22/16.
//

#include "Planets.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"
#include "../GameData.h"
#include "../Utils.h"
#include "../Tokenizer.h"

void Planets::LoadPlanets(std::string filename) {
    std::shared_ptr<Node> planet_node = SkexmlParser::Parse(filename);
    auto children = planet_node->GetChildren();
    for(auto child : children)
    {
        std::string planet_name = child.first;
        std::string planet_id = child.second->GetAttribute("ID");
        std::string coords = child.second->GetAttribute("Coordinates");
        auto coords_list = Tokenizer::GetAllTokens(coords, ',');
        // TODO: validation (fix this mess in general)
        int id = atoi(planet_id.c_str());
        std::shared_ptr<Planet> planet = BuildPlanet(GameData::PLANET_PATH + planet_name + ".sml", atoi(coords_list[0].c_str()),
                                    id, atoi(coords_list[1].c_str()), atoi(coords_list[2].c_str()));
        AddPlanet(planet);
    }
}

std::shared_ptr<Planet> Planets::BuildPlanet(std::string filename, int id, int x, int y, int z) {
    std::shared_ptr<Node> planet_node = SkexmlParser::Parse(filename);
    std::string planet_name = planet_node->GetName();
    std::shared_ptr<Planet> planet = std::make_shared<Planet>(planet_name, id, x, y, z);
    auto children = planet_node->GetChildren();
    for(auto child: children)
    {
        int north = -1;
        int south = -1;
        int east = -1;
        int west = -1;
        auto room_node = child.second;
        std::string long_desc = room_node->GetAttribute("LongDescription");
        std::string short_desc = room_node->GetAttribute("ShortDescription");
        std::string north_string = room_node->GetAttribute("North");
        std::string south_string = room_node->GetAttribute("South");
        std::string east_string = room_node->GetAttribute("East");
        std::string west_string = room_node->GetAttribute("West");
        if(Utils::IsNumber(north_string))
            north = atoi(north_string.c_str());
        if(Utils::IsNumber(south_string))
            south = atoi(south_string.c_str());
        if(Utils::IsNumber(east_string))
            east = atoi(east_string.c_str());
        if(Utils::IsNumber(west_string))
            west = atoi(west_string.c_str());
        std::shared_ptr<Room> room = std::make_shared<Room>(long_desc, short_desc, north, south, east, west, -1, -1, -1,
                                                            -1, -1, -1);
        planet->AddRoom(room);
    }
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
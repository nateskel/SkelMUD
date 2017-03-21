//
// Created by nate on 3/19/17.
//

#include "Ships.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"
#include "../GameData.h"
#include "../Utils.h"
#include "../Tokenizer.h"
#include <algorithm>

void Ships::LoadShips(std::string filename) {
    std::shared_ptr<Node> ship_node = SkexmlParser::Parse(filename);
    auto children = ship_node->GetChildren();
    for(auto child : children)
    {
        std::string ship_name = child.first;
        std::string ship_id = child.second->GetAttribute("ID");
        std::string ship_health = child.second->GetAttribute("Health");
        std::string ship_shields = child.second->GetAttribute("Shields");
        int id = atoi(ship_id.c_str());
        int health = atoi(ship_health.c_str());
        int shields = atoi(ship_shields.c_str());
        std::shared_ptr<Ship> ship = BuildShip(GameData::SHIP_PATH + ship_name + ".sml", id, health, shields);
        AddShip(ship);
    }
}

std::shared_ptr<Ship> Ships::BuildShip(std::string filename, int id, int health, int shields) {
    std::shared_ptr<Node> ship_node = SkexmlParser::Parse(filename);
    std::string ship_name = ship_node->GetName();
    std::shared_ptr<Ship> ship = std::make_shared<Ship>(ship_name, id, health, shields);
    AreaManager::BuildArea(ship_node, ship);
    return ship;
}

void Ships::AddShip(std::shared_ptr<Ship> ship) {
    m_ship_map[ship->GetID()] = ship;
}

std::map<int, std::shared_ptr<Ship>> Ships::GetShips() {
    return m_ship_map;
}

std::map<int, std::shared_ptr<Ship>> Ships::EnumerateShips() {
    auto ships = GetShips();
    std::map<int, std::shared_ptr<Ship>> ship_map;
    int count = 0;
    for(auto ship : ships) {
        count++;
        ship_map[count] = ship.second;
    }
    return ship_map;
};

void Ships::SaveShip(int ship_id) {
    SaveShip(*m_ship_map[ship_id]);
}

void Ships::SaveShip(Ship ship) {
    std::string filename = GameData::SHIP_PATH + ship.GetName() + ".sml";
    std::shared_ptr<Node> ship_node = std::make_shared<Node>(ship.GetName());
    AreaManager::BuildRoomNodes(ship, ship_node);
    SkexmlParser::BuildSkeXML(filename, ship_node);
}
//
// Created by nate on 7/8/16.
//

#include <string>

#include "Races.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"

void Races::LoadRaces(std::string filename) {
    std::shared_ptr<Node> race_node = SkexmlParser::Parse(filename);
    auto children = race_node->GetChildren();
    for(auto child : children)
    {
        std::string race_name = child.first;
        std::shared_ptr<Node> child_node = child.second;
        std::string base_strength = child_node->GetAttribute("Strength");
        std::string base_endurance = child_node->GetAttribute("Endurance");
        std::string base_intelligence = child_node->GetAttribute("Intelligence");
        std::string base_dexterity = child_node->GetAttribute("Dexterity");
        std::string base_skill = child_node->GetAttribute("Skill");
        AddRace(Race(race_name, base_strength, base_endurance, base_intelligence, base_dexterity, base_skill));
    }
}

void Races::AddRace(Race race) {
    m_race_map[race.getRace_name()] = race;
}

std::map<std::string, Race> Races::GetRaces() {
    return m_race_map;
}
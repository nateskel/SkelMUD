//
// Created by nate on 7/8/16.
//

#include "Characters.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"


void Characters::LoadCharacters(std::string filename) {
    std::shared_ptr<Node> class_node = SkexmlParser::Parse(filename);
    auto children = class_node->GetChildren();
    for(auto child : children)
    {
        std::string character_name = child.first;
        std::shared_ptr<Node> child_node = child.second;
        std::string char_class = child_node->GetAttribute("Class");
        std::string race = child_node->GetAttribute("Race");
        std::string str = child_node->GetAttribute("STR");
        std::string dex = child_node->GetAttribute("DEX");
        std::string intel = child_node->GetAttribute("INT");
        std::string end = child_node->GetAttribute("END");
        std::string skill = child_node->GetAttribute("SKILL");
        std::string att_points = child_node->GetAttribute("Attribute Points");
        std::shared_ptr<Player> character = std::make_shared<Player>(-1, character_name, char_class, race);
        character->SetStrength(stoi(str));
        character->SetDexterity(stoi(dex));
        character->SetIntelligence(stoi(intel));
        character->SetEndurance(stoi(end));
        character->SetSkill(stoi(skill));
        character->SetAttributePoints(stoi(att_points));
        AddCharacter(character);
    }
}

void Characters::SaveCharacters(std::string filename) {
    std::string node_name = "Characters";
    std::shared_ptr<Node> parent = std::make_shared<Node>(node_name);
    for(auto character: m_character_map)
    {
        std::string character_name = character.first;
        std::shared_ptr<Player> player = character.second;
        std::shared_ptr<Node> child = std::make_shared<Node>(character_name);
        child->AddAttribute("Class", player->GetPlayerClass());
        child->AddAttribute("Race", player->GetPlayerRaceStr());
        child->AddAttribute("STR", std::to_string(player->GetStrength()));
        child->AddAttribute("DEX", std::to_string(player->GetDexterity()));
        child->AddAttribute("INT", std::to_string(player->GetIntelligence()));
        child->AddAttribute("END", std::to_string(player->GetEndurance()));
        child->AddAttribute("SKILL", std::to_string(player->GetSkill()));
        child->AddAttribute("Attribute Points", std::to_string(player->GetAttributePoints()));
        parent->AddChild(child);
    }
    SkexmlParser::BuildSkeXML(filename, parent);
}

void Characters::AddCharacter(std::shared_ptr<Player> player) {
    m_character_map[player->GetName()] = player;
}

std::map<std::string, std::shared_ptr<Player>> Characters::GetCharacters() {
    return m_character_map;
}

std::map<int, std::shared_ptr<Player>> Characters::EnumerateCharacters() {
    auto characters = GetCharacters();
    std::map<int, std::shared_ptr<Player>> character_map;
    int count = 0;
    for(auto character : characters) {
        count++;
        character_map[count] = character.second;
    }
    return character_map;
}
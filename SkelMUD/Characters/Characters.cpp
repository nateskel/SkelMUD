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
        std::shared_ptr<Player> character = std::make_shared<Player>(-1, character_name, char_class, race);
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
        child->AddAttribute("Race", player->GetPlayerRace());
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
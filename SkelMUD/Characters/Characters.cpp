//
// Created by nate on 7/8/16.
//

#include "Characters.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"


void Characters::LoadCharacters(std::string filename) {
    std::shared_ptr<Node> class_node = SkexmlParser::Parse(filename);
    auto children = class_node->GetChildren();
    int count = 0;
    for(auto child : children)
    {
        std::string character_name = child.first;
        std::shared_ptr<Node> child_node = child.second;
        std::string char_class = child_node->GetAttribute("Class");
        std::string race = child_node->GetAttribute("Race");
        Player character = Player(count, character_name, char_class, race);
        count++;
        AddCharacter(character);
    }
}

void Characters::AddCharacter(Player player) {
    m_character_map[player.GetName()] = player;
}

std::map<std::string, Player> Characters::GetCharacters() {
    return m_character_map;
}

std::map<int, Player> Characters::EnumerateCharacters() {
    auto characters = GetCharacters();
    std::map<int, Player> character_map;
    int count = 0;
    for(auto character : characters) {
        count++;
        character_map[count] = character.second;
    }
    return character_map;
};
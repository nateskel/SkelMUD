//
// Created by nate on 7/8/16.
//

#include "CharacterClasses.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"


void CharacterClasses::LoadClasses(std::string filename) {
    std::shared_ptr<Node> class_node = SkexmlParser::Parse(filename);
    auto children = class_node->GetChildren();
    for(auto child : children)
    {
        std::string class_name = child.first;
        std::shared_ptr<Node> child_node = child.second;
        CharacterClass character_class = CharacterClass(class_name);
        auto skills = child_node->GetAttributes();
        for(auto skill : skills)
        {
//            if(skill.first == child.first)
//                continue;
            character_class.AddSkill(skill.first, atoi(skill.second.c_str()));
        }
        AddClass(character_class);
    }
}

void CharacterClasses::AddClass(CharacterClass character_class) {
    m_class_map[character_class.GetName()] = character_class;
}

std::map<std::string, CharacterClass> CharacterClasses::GetClasses() {
    return m_class_map;
}

std::map<int, CharacterClass> CharacterClasses::EnumerateClasses() {
    auto classes = GetClasses();
    std::map<int, CharacterClass> class_map;
    int count = 0;
    for(auto char_class : classes) {
        count++;
        class_map[count] = char_class.second;
    }
    return class_map;
};

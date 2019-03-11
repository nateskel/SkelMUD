//
// Created by skelton on 10/24/17.
//

#include "Items.h"
#include "../Skexml/SkexmlParser.h"
#include "../Utils.h"
#include "../Logger.h"
#include "../Tokenizer.h"

void Items::LoadItems(std::string filename) {
    std::vector<std::string> files = Utils::GetFilenames(filename);
    for (std::string file : files) {
        std::shared_ptr<Node> item_node = SkexmlParser::Parse(file);
        auto children = item_node->GetChildren();
        for (auto child : children) {
            std::string item_name = child.first;
            std::shared_ptr<Node> child_node = child.second;
            AddItem(Item(item_name));
        }
    }
}

void Items::AddItem(Item item) {
    m_item_map[item.GetItemName()] = item;
}

std::map<std::string, Item> Items::GetItems() {
    return m_item_map;
}

std::map<int, Item> Items::EnumerateItems() {
    return std::map<int, Item>();
}

Item Items::ParseWeapon(std::shared_ptr<Node> node) {
    std::string w_class = node->GetAttribute("Class");
    if(w_class == "Ranged") {
        return ParseRanged(node);
    } else {
        return ParseMelee(node);
    } // Assume Melee
}

Item Items::ParseRanged(std::shared_ptr<Node> node) {
    std::string name = node->GetName();
    std::string normalhit = node->GetAttribute("NormalHit");
    std::string criticalhit = node->GetAttribute("CriticalHit");
    std::string damage = node->GetAttribute("Damage");
    int min = std::stoi(Tokenizer::GetFirstToken(damage, ',', true));
    int max = std::stoi(Tokenizer::GetFirstToken(damage, ','));
    Item item = Item(name);

    return item;
}

Item Items::ParseMelee(std::shared_ptr<Node> node) {
    std::string name = node->GetName();
    std::string normalhit = node->GetAttribute("NormalHit");
    std::string criticalhit = node->GetAttribute("CriticalHit");
    std::string damage = node->GetAttribute("Damage");
    int min = std::stoi(Tokenizer::GetFirstToken(damage, ',', true));
    int max = std::stoi(Tokenizer::GetFirstToken(damage, ','));
    //MeleeWeapon(name, min, max, normalhit, criticalhit);
}

Item Items::ParseArmor(std::shared_ptr<Node> node) {
    std::string name = node->GetName();
    Item item = Item(name);

    return item;
}

Item Items::ParseConsumable(std::shared_ptr<Node> node) {
    return Item();
}

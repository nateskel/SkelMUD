//
// Created by skelton on 10/24/17.
//

#include "Items.h"
#include "../Skexml/SkexmlParser.h"
#include "../Utils.h"
#include "../Logger.h"
#include "../Tokenizer.h"
#include "Consumable.h"

void Items::LoadItems(std::string filename) {
    std::vector<std::string> files = Utils::GetFilenames(filename);
    for (std::string file : files) {
        std::shared_ptr<Node> item_node = SkexmlParser::Parse(file);
        std::string item_type = item_node->GetName();
        auto children = item_node->GetChildren();
        for (auto child : children) {
            std::string item_name = child.first;
            std::shared_ptr<Node> child_node = child.second;
            std::shared_ptr<Item> item = std::make_shared<Item>(item_name);
            if(item_type == "Consumables")
                ParseConsumable(child_node, item);
//            else if(item_type == "Armor")
//                ParseArmor(child_node, item);
//            else if(item_type == "Weapons")
//                ParseWeapon(child_node, item);
            AddItem(item);
        }
    }
}

void Items::AddItem(std::shared_ptr<Item> item) {
    m_item_map[item->GetItemName()] = item;
}

std::map<std::string, std::shared_ptr<Item>> Items::GetItems() {
    return m_item_map;
}

std::shared_ptr<Item> Items::GetItem(std::string name) {
    return m_item_map[name];
}

std::map<int, std::shared_ptr<Item>> Items::EnumerateItems() {
    return std::map<int, std::shared_ptr<Item>>();
}

std::shared_ptr<Item> Items::ParseWeapon(std::shared_ptr<Node> node, std::shared_ptr<Item> item) {
    std::string w_class = node->GetAttribute("Class");
    if(w_class == "Ranged") {
        return ParseRanged(node);
    } else {
        return ParseMelee(node);
    } // Assume Melee
}

std::shared_ptr<Item> Items::ParseRanged(std::shared_ptr<Node> node) {
    std::string name = node->GetName();
    std::string normalhit = node->GetAttribute("NormalHit");
    std::string criticalhit = node->GetAttribute("CriticalHit");
    std::string damage = node->GetAttribute("Damage");
    int min = std::stoi(Tokenizer::GetFirstToken(damage, ',', true));
    int max = std::stoi(Tokenizer::GetFirstToken(damage, ','));
    std::shared_ptr<Item> item = std::make_shared<Item>(name);

    return item;
}

std::shared_ptr<Item> Items::ParseMelee(std::shared_ptr<Node> node) {
    std::string name = node->GetName();
    std::string normalhit = node->GetAttribute("NormalHit");
    std::string criticalhit = node->GetAttribute("CriticalHit");
    std::string damage = node->GetAttribute("Damage");
    int min = std::stoi(Tokenizer::GetFirstToken(damage, ',', true));
    int max = std::stoi(Tokenizer::GetFirstToken(damage, ','));
    //MeleeWeapon(name, min, max, normalhit, criticalhit);
}

void Items::ParseArmor(std::shared_ptr<Node> node, std::shared_ptr<Item> item) {

}

void Items::ParseConsumable(std::shared_ptr<Node> node, std::shared_ptr<Item> item) {
    auto consumable = std::make_shared<Consumable>();
    std::string hp_str = node->GetAttribute("HP");
    std::string eat = node->GetAttribute("Eat");
    std::string drink = node->GetAttribute("Drink");
    if(hp_str != "") {
        int hp = stoi(hp_str);
        consumable->SetHP(hp);
    }
    consumable->SetEat(eat);
    consumable->SetDrink(drink);
    item->AddMixin("Consumable", consumable);
}

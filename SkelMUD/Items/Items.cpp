//
// Created by skelton on 10/24/17.
//

#include "Items.h"
#include "../Skexml/SkexmlParser.h"
#include "../Utils.h"
#include "../Logger.h"
#include "../Tokenizer.h"
#include "Consumable.h"
#include "Wieldable.h"

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
            int value = stoi(child_node->GetAttribute("Value"));
            item->SetValue(value);
            if(item_type == "Consumables")
                ParseConsumable(child_node, item);
//            else if(item_type == "Armor")
//                ParseArmor(child_node, item);
            else if(item_type == "Weapons")
                ParseWeapon(child_node, item);
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

void Items::ParseWeapon(std::shared_ptr<Node> node, std::shared_ptr<Item> item) {
    auto weapon = std::make_shared<Wieldable>();
    std::string w_class = node->GetAttribute("Class");
    std::string w_type = node->GetAttribute("Type");
    std::string handed = node->GetAttribute("Hand");
    std::string name = node->GetName();
    std::string normalhit = node->GetAttribute("NormalHit");
    std::string normalhitother = node->GetAttribute("NormalHitOther");
    std::string criticalhit = node->GetAttribute("CriticalHit");
    std::string criticalhitother = node->GetAttribute("CriticalHitOther");
    std::string damage = node->GetAttribute("Damage");
    int min = std::stoi(Tokenizer::GetFirstToken(damage, ',', true));
    int max = std::stoi(Tokenizer::GetFirstToken(damage, ','));
    int level = std::stoi(node->GetAttribute("Level"));
    std::string scale = node->GetAttribute("Scale");
    int accuracy = std::stoi(node->GetAttribute("Accuracy"));

    if(w_class == "Ranged")
        weapon->SetWeaponClass(RANGED);
    else
        weapon->SetWeaponClass(MELEE);  // Assume Melee

    if(handed == "Either")
        weapon->SetHandType(EITHER_HAND);
    else if(handed == "Main")
        weapon->SetHandType(MAIN_HAND);
    else if(handed == "Off")
        weapon->SetHandType(OFF_HAND);
    else if(handed == "TwoHand")
        weapon->SetHandType(TWO_HAND);

    weapon->SetWeaponType(w_type);
    weapon->SetNormalHit(normalhit);
    item->SetItemName(name);
    weapon->SetNormalHit(normalhit);
    weapon->SetNormalHitOther(normalhitother);
    weapon->SetCriticalHit(criticalhit);
    weapon->SetCriticalHitOther(criticalhitother);
    weapon->SetMaxDamage(max);
    weapon->SetMinDamage(min);
    weapon->SetLevel(level);
    weapon->SetAccuracy(accuracy);

    if(scale == "Dex")
        weapon->SetScale(DEX);
    else if(scale == "Str")
        weapon->SetScale(STR);
    else if(scale == "End")
        weapon->SetScale(END);
    else if(scale == "Int")
        weapon->SetScale(INT);
    else if(scale == "Skill")
        weapon->SetScale(SKILL);
    item->AddMixin("Wieldable", weapon);
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

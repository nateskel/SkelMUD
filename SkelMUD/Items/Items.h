//
// Created by skelton on 10/24/17.
//

#ifndef SKELMUD_ITEMS_H
#define SKELMUD_ITEMS_H


#include "Item.h"
#include "../Skexml/Node.h"
#include <string>
#include <map>
#include <bits/shared_ptr.h>

class Items {
private:
    std::map<std::string, std::shared_ptr<Item>> m_item_map;
    void ParseWeapon(std::shared_ptr<Node> node, std::shared_ptr<Item> item);
    void ParseArmor(std::shared_ptr<Node> node, std::shared_ptr<Item> item);
    void ParseConsumable(std::shared_ptr<Node> node, std::shared_ptr<Item> item);
public:
    void LoadItems(std::string filename);
    void AddItem(std::shared_ptr<Item> item);
    std::map<std::string, std::shared_ptr<Item>> GetItems();
    std::shared_ptr<Item> GetItem(std::string name);
    std::map<int, std::shared_ptr<Item>> EnumerateItems();
};


#endif //SKELMUD_ITEMS_H

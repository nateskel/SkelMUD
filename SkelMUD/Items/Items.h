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
    std::map<std::string, Item> m_item_map;
    Item ParseWeapon(std::shared_ptr<Node> node);
    Item ParseRanged(std::shared_ptr<Node> node);
    Item ParseMelee(std::shared_ptr<Node> node);
    Item ParseArmor(std::shared_ptr<Node> node);
    Item ParseConsumable(std::shared_ptr<Node> node);
public:
    void LoadItems(std::string filename);
    void AddItem(Item item);
    std::map<std::string, Item> GetItems();
    std::map<int, Item> EnumerateItems();
};


#endif //SKELMUD_ITEMS_H

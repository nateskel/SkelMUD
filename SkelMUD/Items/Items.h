//
// Created by skelton on 10/24/17.
//

#ifndef SKELMUD_ITEMS_H
#define SKELMUD_ITEMS_H


#include "Item.h"
#include <string>
#include <map>

class Items {
private:
    std::map<std::string, Item> m_item_map;
public:
    void LoadItems(std::string filename);
    void AddItem(Item item);
    std::map<std::string, Item> GetItems();
    std::map<int, Item> EnumerateItems();
};


#endif //SKELMUD_ITEMS_H

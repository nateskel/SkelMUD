//
// Created by drax on 7/31/19.
//

#ifndef SKELMUD_ITEMCONTAINER_H
#define SKELMUD_ITEMCONTAINER_H

#include <map>
#include <string>
#include "Item.h"

class ItemContainer {
protected:
    std::map<std::string, int> m_items;
public:
    void AddItem(std::string item);
    void AddItem(std::string item, int quantity);
    void AddItem(std::shared_ptr<Item> item);
    void AddItem(std::shared_ptr<Item> item, int quantity);
    void RemoveItem(std::string item);
    std::map<std::string, int> GetItems();

};


#endif //SKELMUD_ITEMCONTAINER_H

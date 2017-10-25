//
// Created by skelton on 10/22/17.
//

#ifndef SKELMUD_ITEM_H
#define SKELMUD_ITEM_H


#include <string>

class Item {

private:
    std::string m_item_name;

public:
    Item();
    Item(std::string item_name);
    std::string GetItemName();
    void SetItemName(std::string name);
};


#endif //SKELMUD_ITEM_H

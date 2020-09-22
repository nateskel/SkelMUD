//
// Created by skelton on 10/22/17.
//

#ifndef SKELMUD_ITEM_H
#define SKELMUD_ITEM_H


#include <string>
#include <map>
#include <memory>
#include "ItemMixin.h"
#include "../Mixin.h"

class Item : public MixinContainer{

private:
    std::string m_item_name;
    int m_value;

public:
    Item();
    Item(std::string item_name);
    Item(std::string item_name, int value);
    std::string GetItemName();
    void SetItemName(std::string name);
    void SetValue(int value);
    int GetValue();
};


#endif //SKELMUD_ITEM_H

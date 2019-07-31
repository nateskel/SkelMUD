//
// Created by skelton on 10/22/17.
//

#ifndef SKELMUD_ITEM_H
#define SKELMUD_ITEM_H


#include <string>
#include <map>
#include <memory>
#include "ItemMixin.h"

class Item {

private:
    std::string m_item_name;
    std::map<std::string, std::shared_ptr<ItemMixin>> m_mixins;
    int m_value;

public:
    Item();
    Item(std::string item_name);
    Item(std::string item_name, int value);
    std::string GetItemName();
    void SetItemName(std::string name);
    void AddMixin(std::string mixin_name, std::shared_ptr<ItemMixin> mixin);
    bool HasMixin(std::string mixin_name);
    std::shared_ptr<ItemMixin> GetMixin(std::string mixin);
    std::map<std::string, std::shared_ptr<ItemMixin>> GetMixins();
    void SetValue(int value);
    int GetValue();
};


#endif //SKELMUD_ITEM_H

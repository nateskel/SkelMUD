//
// Created by skelton on 10/22/17.
//

#include <memory>
#include "Item.h"

void Item::SetItemName(std::string name) {
    m_item_name = name;
}

std::string Item::GetItemName() {
    return m_item_name;
}

Item::Item(std::string item_name) {
    m_item_name = item_name;
    m_mixins = {};
}

Item::Item() {
    m_item_name = "";
}

void Item::AddMixin(std::string mixin_name, std::shared_ptr<ItemMixin> mixin) {
    m_mixins[mixin_name] = mixin;
}

bool Item::HasMixin(std::string mixin_name) {
    auto mixin = m_mixins.find(mixin_name);
    return mixin != m_mixins.end();
}

std::shared_ptr<ItemMixin> Item::GetMixin(std::string mixin) {
    return m_mixins[mixin];
}

std::map<std::string, std::shared_ptr<ItemMixin>> Item::GetMixins() {
    return std::map<std::string, std::shared_ptr<ItemMixin>>();
}

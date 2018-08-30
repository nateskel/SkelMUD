//
// Created by skelton on 10/22/17.
//

#include "Item.h"

void Item::SetItemName(std::string name) {
    m_item_name = name;
}

std::string Item::GetItemName() {
    return m_item_name;
}

Item::Item(std::string item_name) {
    m_item_name = item_name;
}

Item::Item() {
    m_item_name = "";
}

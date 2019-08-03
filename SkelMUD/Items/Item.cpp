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
    m_value = 0;
}
Item::Item(std::string item_name, int value) {
    m_item_name = item_name;
    m_value = value;
}

Item::Item() : MixinContainer() {
    m_item_name = "";
}

void Item::SetValue(int value) {
 m_value = value;
}

int Item::GetValue() {
    return m_value;
}

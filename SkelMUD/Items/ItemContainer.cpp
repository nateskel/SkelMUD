//
// Created by drax on 7/31/19.
//

#include "ItemContainer.h"

void ItemContainer::AddItem(std::string item) {
    if(m_items.find(item) == m_items.end()) {
        m_items[item] = 1;
    }
    else {
        m_items[item]++;
    }
}

void ItemContainer::AddItem(std::shared_ptr<Item> item) {
    AddItem(item->GetItemName());
}

void ItemContainer::RemoveItem(std::string item) {
    if(m_items.find(item) != m_items.end()) {
        m_items[item]--;
        if(m_items[item] == 0) {
            m_items.erase(item);
        }
    }
}

void ItemContainer::AddItem(std::string item, int quantity) {
    if(m_items.find(item) == m_items.end()) {
        m_items[item] = quantity;
    }
    else {
        m_items[item] += quantity;
    }
}

void ItemContainer::AddItem(std::shared_ptr<Item> item, int quantity) {
    AddItem(item->GetItemName(), quantity);
}

std::map<std::string, int> ItemContainer::GetItems() {
    return m_items;
}

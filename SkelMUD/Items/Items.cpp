//
// Created by skelton on 10/24/17.
//

#include "Items.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"
#include "../Utils.h"
#include "../Logger.h"

void Items::LoadItems(std::string filename) {
    std::vector<std::string> files = Utils::GetFilenames(filename);
    for (std::string file : files) {
        std::shared_ptr<Node> race_node = SkexmlParser::Parse(file);
        auto children = race_node->GetChildren();
        for (auto child : children) {
            std::string item_name = child.first;
            std::shared_ptr<Node> child_node = child.second;
            AddItem(Item(item_name));
        }
    }
}

void Items::AddItem(Item item) {
    m_item_map[item.GetItemName()] = item;
}

std::map<std::string, Item> Items::GetItems() {
    return m_item_map;
}

std::map<int, Item> Items::EnumerateItems() {
    return std::map<int, Item>();
}

//
// Created by nate on 8/22/18.
//

#include "NPC.h"

NPC::NPC() : Entity() {
    m_name = "";
    m_is_shopkeeper = false;
};

NPC::NPC(std::string name, bool is_shopkeeper) {
    m_name = name;
    m_is_shopkeeper = is_shopkeeper;
}

bool NPC::IsShopKeeper() {
    return m_is_shopkeeper;
}
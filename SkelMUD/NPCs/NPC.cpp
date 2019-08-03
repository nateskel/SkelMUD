//
// Created by nate on 8/22/18.
//

#include "NPC.h"

NPC::NPC() : Entity() {
    m_name = "";
};

NPC::NPC(std::string name) {
    m_name = name;
}

bool NPC::IsShopKeeper() {
    return HasMixin("Shopkeeper");
}

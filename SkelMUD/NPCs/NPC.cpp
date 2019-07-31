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

void NPC::AddMixin(std::string mixin_name, std::shared_ptr<NPCMixin> mixin) {
    m_mixins[mixin_name] = mixin;
}

bool NPC::HasMixin(std::string mixin_name) {
    auto mixin = m_mixins.find(mixin_name);
    return mixin != m_mixins.end();
}

std::shared_ptr<NPCMixin> NPC::GetMixin(std::string mixin) {
    return m_mixins[mixin];
}

std::map<std::string, std::shared_ptr<NPCMixin>> NPC::GetMixins() {
    return std::map<std::string, std::shared_ptr<NPCMixin>>();
}
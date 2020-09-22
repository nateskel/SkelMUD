//
// Created by drax on 8/1/19.
//

#include "Mixin.h"

Mixin::~Mixin() {

}

void MixinContainer::AddMixin(std::string mixin_name, std::shared_ptr<Mixin> mixin) {
    m_mixins[mixin_name] = mixin;
}

bool MixinContainer::HasMixin(std::string mixin_name) {
    auto mixin = m_mixins.find(mixin_name);
    return mixin != m_mixins.end();
}

std::shared_ptr<Mixin> MixinContainer::GetMixin(std::string mixin) {
    return m_mixins[mixin];
}

std::map<std::string, std::shared_ptr<Mixin>> MixinContainer::GetMixins() {
    return std::map<std::string, std::shared_ptr<Mixin>>();
}

MixinContainer::MixinContainer() {
    m_mixins = {};
}

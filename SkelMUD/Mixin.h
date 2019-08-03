//
// Created by drax on 8/1/19.
//

#ifndef SKELMUD_MIXIN_H
#define SKELMUD_MIXIN_H

#include <memory>
#include <map>

class Mixin {
public:
    virtual ~Mixin();
};


class MixinContainer {
protected:
    std::map<std::string, std::shared_ptr<Mixin>> m_mixins;
public:
    MixinContainer();
    void AddMixin(std::string mixin_name, std::shared_ptr<Mixin> mixin);
    bool HasMixin(std::string mixin_name);
    std::shared_ptr<Mixin> GetMixin(std::string mixin);
    std::map<std::string, std::shared_ptr<Mixin>> GetMixins();
};


#endif //SKELMUD_MIXIN_H

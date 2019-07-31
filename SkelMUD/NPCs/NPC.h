//
// Created by Nate on 8/22/18.
//

#ifndef SKELMUD_NPC_H
#define SKELMUD_NPC_H


#include "../Entity.h"
#include "NPCMixin.h"
#include <string>

class NPC : public Entity {
public:
    NPC();
    NPC(std::string name);
    bool IsShopKeeper();
    void AddMixin(std::string mixin_name, std::shared_ptr<NPCMixin> mixin);
    bool HasMixin(std::string mixin_name);
    std::shared_ptr<NPCMixin> GetMixin(std::string mixin);
    std::map<std::string, std::shared_ptr<NPCMixin>> GetMixins();

private:
    std::map<std::string, std::shared_ptr<NPCMixin>> m_mixins;
};


#endif //SKELMUD_NPC_H

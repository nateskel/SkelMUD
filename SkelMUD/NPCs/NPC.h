//
// Created by Nate on 8/22/18.
//

#ifndef SKELMUD_NPC_H
#define SKELMUD_NPC_H


#include "../Entity.h"
#include "../Mixin.h"
#include <string>

class NPC : public Entity, public MixinContainer {
public:
    NPC();
    NPC(std::string name);
    bool IsShopKeeper();

private:
};


#endif //SKELMUD_NPC_H

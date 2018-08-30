//
// Created by Nate on 8/22/18.
//

#ifndef SKELMUD_NPC_H
#define SKELMUD_NPC_H


#include "../Entity.h"
#include <string>

class NPC : public Entity {
public:
    NPC();
    NPC(std::string name, bool is_shopkeeper);
    bool IsShopKeeper();
private:
    bool m_is_shopkeeper;
};


#endif //SKELMUD_NPC_H

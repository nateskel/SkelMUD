//
// Created by nate on 7/31/19.
//

#ifndef SKELMUD_MOB_H
#define SKELMUD_MOB_H


#include "../Entity.h"
#include "../Mixin.h"

class MOB : public Entity, public MixinContainer {
private:
    int m_xp;
    int m_damage_min;
    int m_damage_max;
};


#endif //SKELMUD_MOB_H

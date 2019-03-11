//
// Created by drax on 2/28/19.
//

#ifndef SKELMUD_WEARABLE_H
#define SKELMUD_WEARABLE_H


#include "Modifier.h"

class Wearable {
public:
    enum class Position { HEAD, ARMS, CHEST, FEET, HANDS, WAIST };
    enum class Type { CLOTH, LIGHT, MEDIUM, HEAVY };

    Wearable(Position pos);

    Position GetPosition() const;

    void SetPosition(Position m_pos);

private:
    Position m_pos;
    Modifier m_modifier;
};


#endif //SKELMUD_WEARABLE_H

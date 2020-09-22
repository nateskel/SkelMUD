//
// Created by drax on 2/28/19.
//

#include "Wearable.h"

Wearable::Wearable(Position pos) {
    m_pos = pos;
    m_modifier = Modifier();
}

Wearable::Position Wearable::GetPosition() const {
    return m_pos;
}

void Wearable::SetPosition(Wearable::Position m_pos) {
    Wearable::m_pos = m_pos;
}

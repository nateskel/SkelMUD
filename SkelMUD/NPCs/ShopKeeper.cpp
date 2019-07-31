//
// Created by drax on 7/31/19.
//

#include "ShopKeeper.h"
#include <math.h>

int ShopKeeper::GetBuyCost(int value) {
    return (int)round(value + value * m_buy_modifier);
}

int ShopKeeper::GetSellCost(int value) {
    return (int)round(value + value * m_sell_modifier);
}

ShopKeeper::ShopKeeper() : NPCMixin(), ItemContainer() {
    m_buy_modifier = 0.10;
    m_sell_modifier = 0.10;
}

#ifndef SKELMUD_SHOPKEEPER_H
#define SKELMUD_SHOPKEEPER_H


#include "NPCMixin.h"
#include "../Items/Item.h"
#include "../Items/ItemContainer.h"
#include <list>
#include <tuple>

class ShopKeeper : public NPCMixin, public ItemContainer {
public:
    ShopKeeper();
    int GetBuyCost(int value);
    int GetSellCost(int value);

private:
    float m_buy_modifier;
    float m_sell_modifier;
};


#endif //SKELMUD_SHOPKEEPER_H

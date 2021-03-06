//
// Created by Nate on 8/22/18.
//

#ifndef SKELMUD_NPCS_H
#define SKELMUD_NPCS_H

#include "NPC.h"
#include "ShopKeeper.h"
#include "../Skexml/Node.h"
#include <string>
#include <map>

class NPCs {
private:
    std::map<std::string, std::shared_ptr<NPC>> m_npc_map;
public:
    void LoadNPCs(std::string filename);
    void AddNPC(std::shared_ptr<NPC> npc);
    std::map<std::string, std::shared_ptr<NPC>> GetNPCs();
    std::map<int, std::shared_ptr<NPC>> EnumerateNPCs();
    ShopKeeper CreateShopKeeper(std::shared_ptr<Node> sk_node);
};


#endif //SKELMUD_NPCS_H

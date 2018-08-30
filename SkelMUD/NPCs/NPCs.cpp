//
// Created by drax on 8/22/18.
//

#include "NPCs.h"
#include "../Skexml/Node.h"
#include "../Skexml/SkexmlParser.h"
#include "../Utils.h"

void NPCs::LoadNPCs(std::string filename) {
    std::vector<std::string> files = Utils::GetFilenames(filename);
    for(auto file: files) {
        std::shared_ptr<Node> npc_node = SkexmlParser::Parse(file);
        std::string npc_name = npc_node->GetName();
        bool is_shopkeeper = npc_node->GetAttribute("ShopKeeper") == "True";
        int room_id = atoi(npc_node->GetAttribute("Room").c_str());
        int location_id = atoi(npc_node->GetAttribute("Planet").c_str());
        auto npc = std::make_shared<NPC>(npc_name, is_shopkeeper);
        npc->SetInShip(false);
        npc->SetRoomID(room_id);
        npc->SetLocationID(location_id);
        AddNPC(npc);
    }
//    std::shared_ptr<Node> npc_node = SkexmlParser::Parse(filename);
//    auto children = npc_node->GetChildren();
//    for(auto child : children)
//    {

//        std::string race_name = child.first;
//        std::shared_ptr<Node> child_node = child.second;
//        std::string base_strength = child_node->GetAttribute("Strength");
//        std::string base_endurance = child_node->GetAttribute("Endurance");
//        std::string base_intelligence = child_node->GetAttribute("Intelligence");
//        std::string base_dexterity = child_node->GetAttribute("Dexterity");
//        std::string base_skill = child_node->GetAttribute("Skill");
//    }
}

void NPCs::AddNPC(std::shared_ptr<NPC> npc) {
    m_npc_map[npc->GetName()] = npc;
}

std::map<std::string, std::shared_ptr<NPC>> NPCs::GetNPCs() {
    return m_npc_map;
};

std::map<int, std::shared_ptr<NPC>> NPCs::EnumerateNPCs() {
    auto npcs = GetNPCs();
    std::map<int, std::shared_ptr<NPC>> npc_map;
    int count = 0;
    for(auto npc : npcs) {
        count++;
        npc_map[count] = npc.second;
    }
    return npc_map;
};
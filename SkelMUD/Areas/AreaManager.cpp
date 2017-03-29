//
// Created by skelton on 12/22/16.
//

#include "AreaManager.h"
#include "../Skexml/SkexmlParser.h"
#include "../Tokenizer.h"
#include "../GameData.h"
#include "../Utils.h"
#include "../Logger.h"



std::shared_ptr<Area> AreaManager::BuildArea(std::shared_ptr<Node> area_node, std::shared_ptr<Area> area) {
    auto children = area_node->GetChildren();
    for(auto child: children)
    {
        int north = -1;
        int south = -1;
        int east = -1;
        int west = -1;
        auto room_node = child.second;
        auto long_desc_list = room_node->GetListAttribute("LongDescription");
        std::stringstream ss;
        for(auto item: long_desc_list) {
            ss << item << "\r\n";
        }
        std::string long_desc = ss.str();
        std::string short_desc = room_node->GetAttribute("ShortDescription");
        std::string landing_level = room_node->GetAttribute("Landing");
        auto ships = room_node->GetListAttribute("Ships");
        std::string north_string = room_node->GetAttribute("North");
        std::string south_string = room_node->GetAttribute("South");
        std::string east_string = room_node->GetAttribute("East");
        std::string west_string = room_node->GetAttribute("West");
        if(Utils::IsNumber(north_string))
            north = atoi(north_string.c_str());
        if(Utils::IsNumber(south_string))
            south = atoi(south_string.c_str());
        if(Utils::IsNumber(east_string))
            east = atoi(east_string.c_str());
        if(Utils::IsNumber(west_string))
            west = atoi(west_string.c_str());
        std::shared_ptr<Room> room = std::make_shared<Room>(long_desc,
                                                            short_desc,
                                                            north,
                                                            south,
                                                            east,
                                                            west,
                                                            -1,
                                                            -1,
                                                            -1,
                                                            -1,
                                                            -1,
                                                            -1);
        if(landing_level == "") {
            room->SetLandingLevel(Room::LandingLevel::NONE);
        }
        else {
            room->SetLandingLevel(m_landing_level_map[landing_level]);
            for (auto ship: ships) {
                room->AddShipID(atoi(ship.c_str()));
            }
        }
        area->AddRoom(room);
    }
    return area;
}

std::shared_ptr<Node> AreaManager::BuildRoomNodes(Area &area, std::shared_ptr<Node> area_node) {
    for(auto room: area.GetRooms()) {
        std::shared_ptr<Node> room_node = std::make_shared<Node>(std::to_string(room->GetID()));
        room_node->AddAttribute("ShortDescription", room->GetShortDescription());
        auto long_desc = Tokenizer::GetAllTokens(room->GetLongDescription(), '\n');
        room_node->AddList("LongDescription", long_desc);
        AddDirectionAttribute("North", room->GetNorth(), room_node);
        AddDirectionAttribute("South", room->GetSouth(), room_node);
        AddDirectionAttribute("East", room->GetEast(), room_node);
        AddDirectionAttribute("West", room->GetWest(), room_node);
        area_node->AddChild(room_node);
    }
    return area_node;
}

void AreaManager::AddDirectionAttribute(std::string direction, int room_id_dest, std::shared_ptr<Node> room_node) {
    if(room_id_dest != -1)
    {
        room_node->AddAttribute(direction, std::to_string(room_id_dest));
    }
}
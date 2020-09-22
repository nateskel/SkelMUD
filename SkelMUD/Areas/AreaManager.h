//
// Created by skelton on 12/22/16.
//

#ifndef SKELMUD_AREA_MANAGER_H
#define SKELMUD_AREA_MANAGER_H


#include "Area.h"
#include "../Skexml/Node.h"
#include <map>

static std::map<std::string, Room::LandingLevel> m_landing_level_map = {
        {"1", Room::LandingLevel::SMALL},
        {"2", Room::LandingLevel::MEDIUM},
        {"3", Room::LandingLevel::LARGE},
        {"4", Room::LandingLevel::CAPITAL},
        {"5", Room::LandingLevel::SUPER_CAPITAL}
};

class AreaManager {
private:
    void static AddDirectionAttribute(std::string direction, int room_id_dest, std::shared_ptr<Node> room_node);
public:
    std::shared_ptr<Area> static BuildArea(std::shared_ptr<Node> area_node, std::shared_ptr<Area> area);
    std::shared_ptr<Node> static BuildRoomNodes(Area &area, std::shared_ptr<Node> area_node);
};


#endif //SKELMUD_AREA_MANAGER_H

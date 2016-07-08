//
// Created by nate on 7/8/16.
//

#ifndef SKELMUD_RACES_H
#define SKELMUD_RACES_H

#include <string>
#include "Race.h"

class Races {
private:
    std::map<std::string, Race> m_race_map;
public:
    void LoadRaces(std::string filename);
    void AddRace(Race race);
    std::map<std::string, Race> GetRaces();
};


#endif //SKELMUD_RACES_H

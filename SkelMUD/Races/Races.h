//
// Created by nate on 7/8/16.
//

#ifndef SKELMUD_RACES_H
#define SKELMUD_RACES_H

#include <string>
#include <memory>
#include "Race.h"

class Races {
private:
    std::map<std::string, std::shared_ptr<Race>> m_race_map;
public:
    void LoadRaces(std::string filename);
    void AddRace(std::shared_ptr<Race> race);
    std::map<std::string, std::shared_ptr<Race>> GetRaces();
    std::map<int, std::shared_ptr<Race>> EnumerateRaces();
};


#endif //SKELMUD_RACES_H

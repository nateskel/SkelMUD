//
// Created by skelton on 10/19/16.
//

#ifndef SKELMUD_CHARACTERS_H
#define SKELMUD_CHARACTERS_H

#include <map>
#include "../Player.h"

class Characters {

private:
    std::map<std::string, Player> m_character_map;
public:
    void LoadCharacters(std::string filename);
    void AddCharacter(Player player);
    std::map<std::string, Player> GetCharacters();
    std::map<int, Player> EnumerateCharacters();
};


#endif //SKELMUD_CHARACTERS_H

//
// Created by skelton on 10/19/16.
//

#ifndef SKELMUD_CHARACTERS_H
#define SKELMUD_CHARACTERS_H

#include <map>
#include <memory>
#include "../Player.h"

class Characters {

private:
    std::map<std::string, std::shared_ptr<Player>> m_character_map;
public:
    void LoadCharacters(std::string filename);
    void SaveCharacters(std::string filename);
    void AddCharacter(std::shared_ptr<Player> player);
    std::map<std::string, std::shared_ptr<Player>> GetCharacters();
    std::map<int, std::shared_ptr<Player>> EnumerateCharacters();
};


#endif //SKELMUD_CHARACTERS_H

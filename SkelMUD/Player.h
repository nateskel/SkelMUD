#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include <string>

class Player : public Entity {
public:
    Player();

    Player(int player_id, std::string player_name, std::string player_class, std::string player_race);

    Player(int player_id, std::string player_name);

    int GetID();

    const std::string &GetPlayerName() const;

    void SetPlayerName(const std::string &name);

    const std::string &GetPlayerClass() const;

    void SetPlayerClass(const std::string &character_class);

    const std::string &GetPlayerRace() const;

    void SetPlayerRace(const std::string &race);


protected:
    int m_player_id;
    std::string m_player_name;
    std::string m_class;
    std::string m_race;

private:
};

#endif // PLAYER_H

#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Items/Item.h"
#include "Races/Race.h"
#include "Items/ItemContainer.h"
#include <string>
#include <memory>
#include <sstream>

class Planet;

class Player : public Entity, public ItemContainer {
public:
    Player();
    //Player(int player_id, std::string player_name, std::string player_class, Race player_race);
    Player(int player_id, std::string player_name, std::string player_class, std::string player_race);
    Player(int player_id, std::string player_name);
    int GetID();
    void SetID(int id);
    const std::string &GetPlayerName() const;
    void SetPlayerName(const std::string &name);
    const std::string &GetPlayerClass() const;
    void SetPlayerClass(const std::string &character_class);
    const std::string &GetPlayerRaceStr() const;
    const std::shared_ptr<Race> &GetPlayerRace() const;
    void SetPlayerRaceStr(const std::string &race);
    void SetPlayerRace(const std::shared_ptr<Race> & race);
    bool IsVisible();
    void SetVisible(const bool visible);
//    void AddItem(std::string item);
//    void AddItem(std::shared_ptr<Item> item);
//    void RemoveItem(std::string item);
    void Send(std::string data);
    std::string GetCommData();
    void SetMainHand(std::shared_ptr<Item> item);
    std::shared_ptr<Item> GetMainHand();
    void SetOffHand(std::shared_ptr<Item> item);
    std::shared_ptr<Item> GetOffHand();
    int GetStrength();
    int GetNetStrength();
    void SetStrength(int value);
    int GetEndurance();
    int GetNetEndurance();
    void SetEndurance(int value);
    int GetIntelligence();
    int GetNetIntelligence();
    void SetIntelligence(int value);
    int GetDexterity();
    int GetNetDexterity();
    void SetDexterity(int value);
    int GetSkill();
    int GetNetSkill();
    void SetSkill(int value);
    int GetAttributePoints() const;
    void SetAttributePoints(int m_attribute_points);
    int GetCredits() const;
    void SetCredits(int value);
    void AddCredits(int amount);
    void RemoveCredits(int amount);

protected:
    int m_player_id;
    bool m_visible;
    std::string m_player_name;
    std::string m_class;
    std::string m_race_str;
    std::shared_ptr<Race> m_race;
    std::stringstream m_send_queue;
    std::shared_ptr<Item> m_main_hand_item;
    std::shared_ptr<Item> m_off_hand_item;
    int m_strength;
    int m_endurance;
    int m_intelligence;
    int m_dexterity;
    int m_skill;
    int m_attribute_points;
    int m_credits;
    int m_xp;
    int m_xp_next_level;

private:
//    std::map<std::string, int> m_items;
};

#endif // PLAYER_H

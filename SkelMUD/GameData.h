//
// Created by nate on 11/18/15.
//

#ifndef SKELMUD_GAMEDATA_H
#define SKELMUD_GAMEDATA_H


#include <map>
#include "Accounts/Accounts.h"
#include "Accounts/Account.h"
#include "Races/Races.h"
#include "Classes/CharacterClasses.h"
#include "Characters/Characters.h"
#include "Planets/Planets.h"
#include "Config/Configuration.h"
#include "Ships/Ships.h"
#include "Items/Items.h"
#include "NPCs/NPCs.h"

class Connection;

class GameData {

public:
    static const std::string BASE_PATH;
    static const std::string ACCOUNT_FILE;
    static const std::string RACE_FILE;
    static const std::string CLASS_FILE;
    static const std::string CHARACTER_FILE;
    static const std::string PLANET_PATH;
    static const std::string PLANET_FILE;
    static const std::string SHIP_PATH;
    static const std::string SHIP_FILE;
    static const std::string CONFIG_FILE;
    static const std::string ACCOUNT_DATA;
    static const std::string ITEM_PATH;
    static const std::string ITEM_FILE;
    static const std::string NPC_PATH;

    GameData();
    void AddConnection(std::shared_ptr<Connection> connection);
    std::shared_ptr<Connection> GetConnection(int index);
    std::map<int, std::shared_ptr<Connection>> GetAllConnections();
    std::map<int, std::shared_ptr<Connection>> GetConnections(std::vector<int>);
    void EraseConnection(int index);
    Accounts GetAccounts();
    Account GetAccount(const std::string& username);
    void AddCharacter(std::string username, std::shared_ptr<Player> character);
    std::shared_ptr<Player> GetPlayer(std::string name);
    std::map<std::string, std::shared_ptr<Player>> GetPlayers();
    Races GetRaces();
    std::shared_ptr<Race> GetRace(std::string race_name);
    CharacterClasses GetClasses();
    void AddAccount(Account account);
    void SaveAccounts(std::string filename);
    void SaveCharacters(std::string filename);
    void SavePlanet(int id);
    void SaveShip(int id);
    std::map<int, std::shared_ptr<Connection>> GetLoggedInConnections();
    std::shared_ptr<Connection> GetConnection(std::string character_name);
    std::shared_ptr<Connection> GetConnectionByUsername(std::string username);
    std::shared_ptr<Planet> GetPlanet(int ID);
    std::shared_ptr<Ship> GetShip(int ID);
    std::shared_ptr<Area> GetArea(int ID, bool is_ship);
    std::shared_ptr<Room> GetRoom(int AreaID, int RoomID, bool is_ship);
    std::shared_ptr<Planet> GetPlanet(std::string name);
    std::shared_ptr<Item> GetItem(std::string name);
    std::shared_ptr<NPC> GetNPC(std::string name);
    Planets& GetPlanets();
    Ships& GetShips();
    Configuration& GetConfiguration();

private:
    void PopulateShips();
    void PopulateNPCs();
    void PopulateRaces();
    std::map<int, std::shared_ptr<Connection>> m_connections;
    Accounts m_accounts;
    Races m_races;
    CharacterClasses m_classes;
    Characters m_characters;
    Planets m_planets;
    Ships m_ships;
    Items m_items;
    NPCs m_npcs;
    Configuration m_configuration;
};


#endif //SKELMUD_GAMEDATA_H

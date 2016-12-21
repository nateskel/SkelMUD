//
// Created by nate on 10/28/16.
//

#ifndef SKELMUD_BUILDINGSTATE_H
#define SKELMUD_BUILDINGSTATE_H

#include "GameState.h"
#include "PlayingState.h"

class BuildingState : public PlayingState {
private:
    static std::map<std::string, PlayingState::Direction> m_direction_map;

public:
    BuildingState(std::shared_ptr<GameData> data) : PlayingState(data) {
        m_cmd_map["createroom"] = &CmdBuildRoom;
        m_cmd_map["linkroom"] = &CmdLink;
        m_cmd_map["getid"] = &CmdGetRoomID;
        m_cmd_map["setlong"] = &CmdSetLongDesc;
        m_cmd_map["setshort"] = &CmdSetShortDesc;
        m_cmd_map["play"] = &CmdPlay;
        m_cmd_map["saveplanet"] = &CmdSavePlanet;
        m_cmd_map["unlink"] = &CmdUnlink;
        m_cmd_map["deleteroom"] = &CmdDeleteRoom;
    }

    virtual void init(std::shared_ptr<Connection> connection) override;
    virtual std::string GetPrompt(std::shared_ptr<Connection> connection) override;
    static void CmdBuildRoom(const std::string& input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data);
    static void CmdDeleteRoom(const std::string& input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data);
    static void CmdLink(const std::string& input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);
    static void CmdUnlink(const std::string& input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);
    static void CmdGetRoomID(const std::string& input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);
    static void CmdSetLongDesc(const std::string& input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data);
    static void CmdSetShortDesc(const std::string& input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data);
    static void CmdPlay(const std::string& input, std::shared_ptr<Connection> connection,
                                std::shared_ptr<GameData> game_data);
    static void CmdSavePlanet(const std::string& input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);
};

#endif //SKELMUD_BUILDINGSTATE_H

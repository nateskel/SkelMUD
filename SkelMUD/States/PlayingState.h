//
// Created by nate on 11/22/15.
//

#ifndef SKELMUD_PLAYINGSTATE_H
#define SKELMUD_PLAYINGSTATE_H


#include "GameState.h"

class PlayingState : public GameState {
public:
    enum Direction {
        NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST, UP, DOWN
    } direction;

    PlayingState(std::shared_ptr<GameData> data) : GameState(data) {
        m_cmd_map["help"] = &CmdHelp;
        m_cmd_map["tell"] = &CmdTell;
        m_cmd_map["chat"] = &CmdChat;
        m_cmd_map["online"] = &CmdOnline;
        m_cmd_map["look"] = &CmdLook;
        m_cmd_map["l"] = &CmdLook;
        m_cmd_map["north"] = &CmdNorth;
        m_cmd_map["n"] = &CmdNorth;
        m_cmd_map["south"] = &CmdSouth;
        m_cmd_map["s"] = &CmdSouth;
        m_cmd_map["east"] = &CmdEast;
        m_cmd_map["e"] = &CmdEast;
        m_cmd_map["west"] = &CmdWest;
        m_cmd_map["w"] = &CmdWest;
        m_cmd_map["up"] = &CmdUp;
        m_cmd_map["u"] = &CmdUp;
        m_cmd_map["down"] = &CmdDown;
        m_cmd_map["d"] = &CmdDown;
        m_cmd_map["build"] = &CmdBuild;
        m_cmd_map["goto"] = &CmdGoto;
        m_cmd_map["open"] = &CmdOpen;
        m_cmd_map["close"] = &CmdClose;
        m_cmd_map["enter"] = &CmdEnter;
        m_cmd_map["leave"] = &CmdLeave;
        m_cmd_map["launch"] = &CmdTakeOff;
        m_cmd_map["say"] = &CmdSay;
        m_cmd_map["emote"] = &CmdEmote;
        m_cmd_map["course"] = &CmdSetCourse;
        m_cmd_map["scan"] = &CmdScan;
        m_cmd_map["orbit"] = &CmdOrbit;
        m_cmd_map["land"] = &CmdLand;
        m_cmd_map["speed"] = &CmdSetSpeed;
        m_cmd_map["quit"] = &CmdQuit;
    }

    virtual void init(std::shared_ptr<Connection> connection) override;

    virtual void processInput(const std::string &input, std::shared_ptr<Connection> connection) override;

    virtual std::string GetPrompt(std::shared_ptr<Connection> connection) override;

    static void CmdHelp(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdTell(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdChat(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdOnline(const std::string &input, std::shared_ptr<Connection> connection,
                          std::shared_ptr<GameData> game_data);

    static void CmdLook(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdNorth(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdSouth(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdEast(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdWest(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdUp(const std::string &input, std::shared_ptr<Connection> connection,
                      std::shared_ptr<GameData> game_data);

    static void CmdDown(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdBuild(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdGoto(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdOpen(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdClose(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdEnter(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdLeave(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdTakeOff(const std::string &input, std::shared_ptr<Connection> connection,
                           std::shared_ptr<GameData> game_data);

    static void CmdSay(const std::string &input, std::shared_ptr<Connection> connection,
                       std::shared_ptr<GameData> game_data);

    static void CmdEmote(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdSetCourse(const std::string &input, std::shared_ptr<Connection> connection,
                             std::shared_ptr<GameData> game_data);

    static void CmdSetSpeed(const std::string &input, std::shared_ptr<Connection> connection,
                            std::shared_ptr<GameData> game_data);

    static void CmdScan(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdOrbit(const std::string &input, std::shared_ptr<Connection> connection,
                         std::shared_ptr<GameData> game_data);

    static void CmdLand(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

    static void CmdQuit(const std::string &input, std::shared_ptr<Connection> connection,
                        std::shared_ptr<GameData> game_data);

protected:
    std::map<std::string, void (*)(const std::string &, std::shared_ptr<Connection>,
                                   std::shared_ptr<GameData>)> m_cmd_map;

    static void Move(std::shared_ptr<Connection> connection, std::shared_ptr<GameData> game_data,
                     Direction direction);

    static bool CheckCockpitCommand(std::shared_ptr<Connection> connection,
                                    std::shared_ptr<GameData> game_data, bool InSpace);

    static std::string GetValidDirections(Room &room);

    static void ChangeSpeed(double speed, std::shared_ptr<Ship> &ship);
};


#endif //SKELMUD_PLAYINGSTATE_H

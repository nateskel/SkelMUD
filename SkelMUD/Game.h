#ifndef GAME_H
#define GAME_H

#include "Connection.h"
#include "Player.h"
#include "Sender.h"
#include "TextProcessor.h"
#include "Planet.h"
#include "Thread.h"
#include "Utils.h"
#include "Logger.h"
#include <map>

class Game
{
public:
	Game();
	~Game();
	void Start();
	void AddOutput(int id, std::string output);
	std::string GetOutput(int id);
	struct Command
	{
		void* (*command_function)(int, std::string, Game*);
		Connection::State state;
	};
	void RegisterCommand(std::string command_string, void* (*command_function)(int, std::string, Game*), Connection::State state);
	bool ProcessCommand(std::string command_string, Connection::State state, int id);

private:
	std::vector<std::string> DIRECTIONS;
	bool m_running;
	HANDLE m_mutex;
	Logger Log = Logger(Logger::L_ERROR);
	std::map<SOCKET, Connection*> m_connection_map;
	std::map<int, Player*> m_player_map;
	std::vector<Planet*> m_planets;
	std::vector<Connection::Account> m_accounts;
	std::vector<Command> m_command_list;
	std::map<int, std::string> m_output_map;
	std::map<std::string, Command> m_command_map;
	static THREAD ListenThread(LPVOID lpParam);
	Sender m_sender;
	OutputManager m_output_manager;

	void processCommand(std::string command, int id);
	void registerCommands();
	std::string createStatusBar(Player* player);
	bool isDirection(std::string input);
	void augmentCommand(Connection::State state, std::string &data);

	static void* processLook(int id, std::string data, Game* game);
	static void* processUsername(int id, std::string data, Game* game);
	static void* processPassword(int id, std::string data, Game* game);
	static void* processSay(int id, std::string data, Game* game);
	static void* processDirectionalCommand(int id, std::string command, Game* game);
	static void* processOOC(int id, std::string data, Game* game);
	static void* processHelp(int id, std::string data, Game* game);
	static void* processQuit(int id, std::string data, Game* game);
	static void* processInventory(int id, std::string data, Game* game);
};

#endif // GAME_H

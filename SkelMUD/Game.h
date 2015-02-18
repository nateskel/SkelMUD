#ifndef GAME_H
#define GAME_H

#include "Connection.h"
#include "Player.h"
#include "Sender.h"
#include "TextProcessor.h"
#include "Planet.h"
#include "Thread.h"
#include <map>

class Game
{
public:
	Game();
	~Game();
	void Start();
	void RemoveEndline(std::string &data);
	void AddOutput(int id, std::string output);
	std::string GetOutput(int id);

private:
	bool m_running;
	HANDLE m_mutex;
	std::map<SOCKET, Connection*> m_connection_map;
	std::map<int, Player*> m_player_map;
	std::vector<Planet*> m_planets;
	std::map<int, std::string> m_output_map;
	static THREAD ListenThread(LPVOID lpParam);
	Sender m_sender;
	OutputManager m_output_manager;

	void processCommand(std::string command, int id);
	bool processDirectionCommand(std::string command, int id);
	void processLook(int id);
	//virtual void recv_callback(char* data, int socket);
};

#endif // GAME_H

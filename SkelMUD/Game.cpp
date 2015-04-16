#include "Game.h"
#include "Tokenizer.h"
#include "File.h"
#include <iostream>
#include <algorithm>
#ifndef _WIN32
#include <unistd.h>
#define Sleep(seconds) sleep(seconds)
#endif

Game::Game()
{
	m_running = false;
#ifdef _WIN32
	m_mutex = CreateMutex(NULL, false, NULL);
#endif
	m_sender = Sender();
	m_output_manager = OutputManager();
	//m_planets.push_back(BuildPlanet());
	//File accountFile = File();
	File planetFile = File();
	//m_accounts = accountFile.LoadAccounts();
	m_planets = planetFile.LoadPlanets();
}

Game::~Game()
{

}

void Game::RegisterCommand(std::string command_string, void* command_function)
{

}

void Game::ProcessCommand(std::string command_string)
{

}

void Game::AddOutput(int id, std::string output)
{
	std::map<int, std::string>::iterator iterator = m_output_map.find(id);
	if (iterator != m_output_map.end())
		iterator->second.append(output);
	else
		m_output_map[id] = output;
}

std::string Game::GetOutput(int id)
{
	std::map<int, std::string>::iterator iterator = m_output_map.find(id);
	if (iterator != m_output_map.end())
		return iterator->second;
	else
		return NULL;
}

void Game::Start()
{
	m_running = true;
	T_HANDLE hThread = Thread::MakeThread(ListenThread, this);
	std::map<SOCKET, Connection*>::iterator it;
	std::map<SOCKET, Connection*>::iterator it_end;
	while (m_running)
	{
		//loop
		Thread::Lock(m_mutex);
		it = m_connection_map.begin();
		it_end = m_connection_map.end();
		Thread::Unlock(m_mutex);
		//int DEBUGINT = 0;
		for (it; it != it_end; )
		{
			int id = it->first;
			Thread::Lock(m_mutex);
			Connection* connection = it->second;
			if (connection == NULL)
				continue;
			Connection::State state = connection->GetState();
			Thread::Unlock(m_mutex);
			if (state == Connection::LOGGEDIN)
			{
				Player* player = m_player_map[id];
				if (player == NULL)
				{
					// Log.Error("Logged in player not found!");
				}
				else
				{
					std::string statusbar = RED;
					statusbar.append("<");
					statusbar.append("HP: ");
					statusbar.append(std::to_string(player->GetHP()));
					statusbar.append("/");
					statusbar.append(std::to_string(player->GetMaxHP()));
					statusbar.append(CYAN);
					statusbar.append(" SP: ");
					statusbar.append(std::to_string(player->GetSP()));
					statusbar.append("/");
					statusbar.append(std::to_string(player->GetMaxSP()));
					statusbar.append(YELLOW);
					statusbar.append(" Stamina: ");
					statusbar.append(std::to_string(player->GetStamina()));
					statusbar.append("/");
					statusbar.append(std::to_string(player->GetMaxStamina()));
					statusbar.append(">");
					statusbar.append(WHITE);
					statusbar.append("\r\n> ");
					connection->Send(statusbar);
				}
			}
			else
				connection->Send("\r\n> ");
			if (state == Connection::DISCONNECTED)
			{
				if (m_player_map.find(id) != m_player_map.end())
				{
					m_planets[m_player_map[id]->GetPlanetID()]->GetRoom(m_player_map[id]->GetRoomID())->RemovePlayer(id);
					delete m_player_map[id];
					m_player_map.erase(id);
				}
				std::map<SOCKET, Connection*>::iterator it_temp = it;
				it++;
				m_connection_map.erase(it_temp);
				continue;
			}
			else if (state == Connection::CONNECTED)
			{
				m_sender.Send(m_output_manager.GetIntroText(), connection, YELLOW);
				connection->SetState(Connection::USERNAME);
				it++;
				continue;
			}
			else
			{
				if (connection->HasMoreData())
				{
					std::string data = connection->GetNextData();
					Utils::RemoveEndline(data);
					switch (state)
					{
					case Connection::USERNAME:
						//TODO: this should be run through character creation
						// the name used here is the username, not the character name (TEMPORARY)
						m_player_map[id] = new Player(id, data);
						m_planets[m_player_map[id]->GetPlanetID()]->GetRoom(m_player_map[id]->GetRoomID())->AddPlayer(m_player_map[id]);
						m_sender.Send("Password: ", connection, YELLOW);
						connection->SetState(Connection::PASSWORD);
						break;
					case Connection::PASSWORD:
						m_sender.Send("Logged in!\r\n", connection);
						connection->SetState(Connection::LOGGEDIN);
						processLook(id);
						break;
					case Connection::LOGGEDIN:
						processCommand(data, id);
						break;
					case Connection::OOC: {
						std::vector<std::string> allTokens = Tokenizer::GetAllTokens(data);
						if (allTokens.size() == 2) {
							if (Tokenizer::UpperCase(allTokens[0]) == "OOC" && Tokenizer::UpperCase(allTokens[1]) == "OFF") {
								m_sender.Send("OOC Off\r\n", m_connection_map[id], MAGENTA);
								m_connection_map[id]->SetState(Connection::LOGGEDIN);
								break;
							}
						}
						data.insert(0, "OOC ");
						processCommand(data, id);
					}
						break;
					default:
						break;
					}
				}
				it++;
			}
		}
		Sleep(1);
	}
}

void Game::processCommand(std::string data, int id)
{
	std::string command = Tokenizer::UpperCase(Tokenizer::GetFirstToken(data));
	std::vector<std::string> commandlist = Tokenizer::GetAllTokens(data);
	if (command == "OOC")
	{
		if (commandlist.size() == 0)
			return;
		if (Tokenizer::UpperCase(commandlist[0]) == "ON")
		{
			m_sender.Send("OOC On\r\n", m_connection_map[id], MAGENTA);
			m_connection_map[id]->SetState(Connection::OOC);
			return;
		}
		std::string output = "\r\n";
		output.append(m_player_map[id]->GetName());
		output.append("(ooc): ");
		output.append(data);
		output.append("\r\n");
		m_sender.SendAll(output, m_connection_map, id, YELLOW);
	}
	else if (command == "SAY")
	{
		Player* player = m_player_map[id];
		int planet_id = player->GetPlanetID();
		int room_id = player->GetRoomID();
		std::vector<int> players = m_planets[planet_id]->GetRoom(room_id)->GetPlayerIDs(id);
		std::string color = BOLD;
		color.append(CYAN);
		std::string prefix = "\r\n";
		prefix.append(m_player_map[id]->GetName());
		prefix.append(": ");
		data.insert(0, prefix);
		data.append("\r\n");
		m_sender.SendToMultiple(data, m_connection_map, players, color);
	}
	else if (command == "HELP")
	{
		m_sender.Send("Commands are not case sensitive\r\n\r\nOOC <message>: Out of character global chat\r\nOOC <ON/OFF>: Turn OOC permanently ON/OFF\r\nSAY <message>: In character local chat.\r\n", m_connection_map[id]);
	}
	else if (command == "QUIT1459")
	{
		m_running = false;
	}
	else if (command == "LOOK" || command == "L")
	{
		processLook(id);
	}
	else if (command == "CON")
	{
		m_sender.Send(std::to_string(m_connection_map.size()), m_connection_map[id]);
	}
	else
	{
		if(!processDirectionalCommand(command, id))
			m_sender.Send("Unrecognized Command. Type 'help' for a list of commands.\r\n", m_connection_map[id]);
	}
}

bool Game::processDirectionalCommand(std::string command, int id)
{
	bool result = false;
	Player* current_player = m_player_map[id];
	int current_planet = current_player->GetPlanetID();
	int current_room = current_player->GetRoomID();
	if (command == "N" || command == "NORTH")
		result = m_planets[current_planet]->MoveNorth(current_room, id);
	else if (command == "S" || command == "SOUTH")
		result = m_planets[current_planet]->MoveSouth(current_room, id);
	else if (command == "E" || command == "EAST")
		result = m_planets[current_planet]->MoveEast(current_room, id);
	else if (command == "W" || command == "WEST")
		result = m_planets[current_planet]->MoveWest(current_room, id);
	else if (command == "NE" || command == "NORTHEAST")
		result = m_planets[current_planet]->MoveNorthEast(current_room, id);
	else if (command == "NW" || command == "NORTHWEST")
		result = m_planets[current_planet]->MoveNorthWest(current_room, id);
	else if (command == "SE" || command == "SOUTHEAST")
		result = m_planets[current_planet]->MoveSouthEast(current_room, id);
	else if (command == "SW" || command == "SOUTHWEST")
		result = m_planets[current_planet]->MoveSouthWest(current_room, id);
	else if (command == "U" || command == "UP")
		result = m_planets[current_planet]->MoveUp(current_room, id);
	else if (command == "D" || command == "DOWN")
		result = m_planets[current_planet]->MoveDown(current_room, id);
	if (!result)
	{
		m_sender.Send("Can't go that way!\r\n", m_connection_map[id]);
	}
	else
	{
		std::vector<int> leaving_players = m_planets[current_planet]->GetRoom(current_room)->GetPlayerIDs(id);
		std::vector<int> arriving_players = m_planets[current_planet]->GetRoom(current_player->GetRoomID())->GetPlayerIDs(id);
		std::string arrive_message = "\r\n";
		arrive_message.append(current_player->GetName());
		arrive_message.append(" entered the room\r\n");
		std::string leave_message = "\r\n";
		leave_message.append(current_player->GetName());
		leave_message.append(" left the room\r\n");
		m_sender.SendToMultiple(arrive_message, m_connection_map, arriving_players);
		m_sender.SendToMultiple(leave_message, m_connection_map, leaving_players);
		processLook(id);
	}
	return result;
}

void Game::processLook(int id)
{
	Player* current_player = m_player_map[id];
	Room* room = m_planets[current_player->GetPlanetID()]->GetRoom(current_player->GetRoomID());
	std::string output = room->GetLongDescription();
	output.append("\r\n");
	output.append(room->GetShortDescription());
	output.append("\r\n\r\n");
	output.append(CYAN);

	std::vector<int> visible_players = room->GetPlayerIDs(id);
	for (int i = 0; i < (int)visible_players.size(); i++)
	{
		output.append(m_player_map[visible_players[i]]->GetName());
		output.append(" is here.\r\n");
	}
	output.append(WHITE);
	output.append("Valid directions are: ");
	if (room->GetNorth() != -1)
		output.append("North ");
	if (room->GetSouth() != -1)
		output.append("South ");
	if (room->GetEast() != -1)
		output.append("East ");
	if (room->GetWest() != -1)
		output.append("West ");
	if (room->GetNorthEast() != -1)
		output.append("NorthEast ");
	if (room->GetNorthWest() != -1)
		output.append("NorthWest ");
	if (room->GetSouthEast() != -1)
		output.append("SouthEast ");
	if (room->GetSouthWest() != -1)
		output.append("SouthWest ");
	if (room->GetUp() != -1)
		output.append("Up ");
	if (room->GetDown() != -1)
		output.append("Down ");
	output.append("\r\n");
	m_sender.Send(output, m_connection_map[id]);
}

THREAD Game::ListenThread(LPVOID lpParam)
{
	Game* game = (Game*)lpParam;

	ServerSocket myListener = ServerSocket(25570); // 25570
	myListener.Initialize();
	int error = myListener.Listen();
	while (game->m_running)
	{
		DataSocket* datasocket = myListener.Accept();
		Connection* connection = new Connection(datasocket);
		connection->Run();
		Thread::Lock(game->m_mutex);
		game->m_connection_map[datasocket->GetSocket()] = connection;
		Thread::Unlock(game->m_mutex);
	}
	game->m_running = false;
	myListener.Close();
	return 0;
}
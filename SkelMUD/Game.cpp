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
	DIRECTIONS = { "NORTH", "N", "SOUTH", "S", "EAST", "E", "WEST", "W", "NORTHEAST", "NE", "NORTHWEST",
		"NW", "SOUTHEAST", "SE", "SOUTHWEST", "SW", "UP", "U", "DOWN", "D" };
	std::sort(DIRECTIONS.begin(), DIRECTIONS.end());
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
	registerCommands();
}

Game::~Game()
{

}

void Game::registerCommands()
{
	RegisterCommand("USERNAME", processUsername, Connection::USERNAME);
	RegisterCommand("PASSWORD", processPassword, Connection::PASSWORD);
	RegisterCommand("SAY", processSay, Connection::LOGGEDIN);
	RegisterCommand("LOOK", processLook, Connection::LOGGEDIN);
	RegisterCommand("L", processLook, Connection::LOGGEDIN);
	RegisterCommand("DIRECTION", processDirectionalCommand, Connection::LOGGEDIN);
	RegisterCommand("OOC", processOOC, Connection::LOGGEDIN);
	RegisterCommand("OOCON", processOOC, Connection::OOC);
	RegisterCommand("QUIT1459", processQuit, Connection::LOGGEDIN);
	RegisterCommand("HELP", processHelp, Connection::LOGGEDIN);
	RegisterCommand("INVENTORY", processInventory, Connection::LOGGEDIN);
}

void Game::augmentCommand(Connection::State state, std::string &data)
{
	if (state == Connection::USERNAME)
	{
		data.insert(0, "USERNAME ");
	}
	else if (state == Connection::PASSWORD)
	{
		data.insert(0, "PASSWORD ");
	}
	else if (state == Connection::OOC)
	{
		data.insert(0, "OOCON ");
	}
	else if (isDirection(data))
	{
		data = Tokenizer::UpperCase(data);
		data.insert(0, "DIRECTION ");
	}
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
					Log.Error("Logged in player not found!");
				}
				else
				{
					std::string statusbar = createStatusBar(player);
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
					augmentCommand(state, data);
					if (!ProcessCommand(data, state, id))
					{
						m_sender.Send("Unrecognized Command. Type 'help' for a list of commands.\r\n", connection);
					}
					else
					{
						m_sender.Send("", connection);
					}
				}
				it++;
			}
		}
		Sleep(1);
	}
}

void Game::RegisterCommand(std::string command_string, void* (*command_function)(int, std::string, Game*), Connection::State state)
{
	Command command;
	command.command_function = command_function;
	command.state = state;
	m_command_map[command_string] = command;
}

bool Game::ProcessCommand(std::string command_string, Connection::State state, int id)
{
	std::string command_input = Tokenizer::UpperCase(Tokenizer::GetFirstToken(command_string));
	if (command_input.size() == 0)
		return false;
	if (m_command_map.find(command_input) == m_command_map.end())
		return false;
	Command command = m_command_map[command_input];
	if (command.state == state)
	{
		command.command_function(id, command_string, this);
		return true;
	}
	return false;
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

void* Game::processDirectionalCommand(int id, std::string command, Game* game)
{
	bool result = false;
	Player* current_player = game->m_player_map[id];
	int current_planet = current_player->GetPlanetID();
	int current_room = current_player->GetRoomID();
	if (command == "N" || command == "NORTH")
		result = game->m_planets[current_planet]->MoveNorth(current_room, id);
	else if (command == "S" || command == "SOUTH")
		result = game->m_planets[current_planet]->MoveSouth(current_room, id);
	else if (command == "E" || command == "EAST")
		result = game->m_planets[current_planet]->MoveEast(current_room, id);
	else if (command == "W" || command == "WEST")
		result = game->m_planets[current_planet]->MoveWest(current_room, id);
	else if (command == "NE" || command == "NORTHEAST")
		result = game->m_planets[current_planet]->MoveNorthEast(current_room, id);
	else if (command == "NW" || command == "NORTHWEST")
		result = game->m_planets[current_planet]->MoveNorthWest(current_room, id);
	else if (command == "SE" || command == "SOUTHEAST")
		result = game->m_planets[current_planet]->MoveSouthEast(current_room, id);
	else if (command == "SW" || command == "SOUTHWEST")
		result = game->m_planets[current_planet]->MoveSouthWest(current_room, id);
	else if (command == "U" || command == "UP")
		result = game->m_planets[current_planet]->MoveUp(current_room, id);
	else if (command == "D" || command == "DOWN")
		result = game->m_planets[current_planet]->MoveDown(current_room, id);
	if (!result)
	{
		game->m_sender.Send("Can't go that way!\r\n", game->m_connection_map[id]);
	}
	else
	{
		std::vector<int> leaving_players = game->m_planets[current_planet]->GetRoom(current_room)->GetPlayerIDs(id);
		std::vector<int> arriving_players = game->m_planets[current_planet]->GetRoom(current_player->GetRoomID())->GetPlayerIDs(id);
		std::string arrive_message = "\r\n";
		arrive_message.append(current_player->GetName());
		arrive_message.append(" entered the room\r\n");
		std::string leave_message = "\r\n";
		leave_message.append(current_player->GetName());
		leave_message.append(" left the room\r\n");
		game->m_sender.SendToMultiple(arrive_message, game->m_connection_map, arriving_players);
		game->m_sender.SendToMultiple(leave_message, game->m_connection_map, leaving_players);
		processLook(id, "", game);
	}
	return 0;
}

void* Game::processLook(int id, std::string data, Game* game)
{
	Player* current_player = game->m_player_map[id];
	Room* room = game->m_planets[current_player->GetPlanetID()]->GetRoom(current_player->GetRoomID());
	std::string output = room->GetLongDescription();
	output.append("\r\n");
	output.append(room->GetShortDescription());
	output.append("\r\n\r\n");
	output.append(CYAN);

	std::vector<int> visible_players = room->GetPlayerIDs(id);
	for (int i = 0; i < (int)visible_players.size(); i++)
	{
		output.append(game->m_player_map[visible_players[i]]->GetName());
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
	game->m_sender.Send(output, game->m_connection_map[id]);
	return 0;
}

void* Game::processUsername(int id, std::string data, Game* game)
{
	game->m_player_map[id] = new Player(id, data);
	game->m_planets[game->m_player_map[id]->GetPlanetID()]->GetRoom(game->m_player_map[id]->GetRoomID())->AddPlayer(game->m_player_map[id]);
	game->m_sender.Send("Password: ", game->m_connection_map[id], YELLOW);
	game->m_connection_map[id]->SetState(Connection::PASSWORD);
	return 0;
}

void* Game::processPassword(int id, std::string data, Game* game)
{
	Connection* connection = game->m_connection_map[id];
	game->m_sender.Send("Logged in!\r\n", connection);
	connection->SetState(Connection::LOGGEDIN);
	processLook(id, "", game);
	return 0;
}

void* Game::processSay(int id, std::string data, Game* game)
{
	Player* player = game->m_player_map[id];
	int planet_id = player->GetPlanetID();
	int room_id = player->GetRoomID();
	std::vector<int> players = game->m_planets[planet_id]->GetRoom(room_id)->GetPlayerIDs(id);
	std::string color = BOLD;
	color.append(CYAN);
	std::string prefix = "\r\n";
	prefix.append(game->m_player_map[id]->GetName());
	prefix.append(": ");
	data.insert(0, prefix);
	data.append("\r\n");
	game->m_sender.SendToMultiple(data, game->m_connection_map, players, color);
	return 0;
}

void* Game::processOOC(int id, std::string data, Game* game)
{
	std::vector<std::string> commandlist = Tokenizer::GetAllTokens(data);
	if (commandlist.size() == 0)
		return 0;
	if (commandlist.size() == 2 && commandlist[0] == "OOC" && Tokenizer::UpperCase(commandlist[1]) == "OFF")
	{
		game->m_sender.Send("OOC Off\r\n", game->m_connection_map[id], MAGENTA);
		game->m_connection_map[id]->SetState(Connection::LOGGEDIN);
		return 0;
	}
	if (Tokenizer::UpperCase(commandlist[0]) == "ON")
	{
		game->m_sender.Send("OOC On\r\n", game->m_connection_map[id], MAGENTA);
		game->m_connection_map[id]->SetState(Connection::OOC);
		return 0;
	}
	std::string output = "\r\n";
	output.append(game->m_player_map[id]->GetName());
	output.append("(OOC): ");
	output.append(data);
	output.append("\r\n");
	game->m_sender.SendAll(output, game->m_connection_map, id, YELLOW);
	return 0;
}

void* Game::processHelp(int id, std::string data, Game* game)
{
	game->m_sender.Send("Commands are not case sensitive\r\n\r\nOOC <message>: Out of character global chat\r\nOOC <ON/OFF>: Turn OOC permanently ON/OFF\r\nSAY <message>: In character local chat.\r\n", game->m_connection_map[id]);
	return 0;
}

void* Game::processQuit(int id, std::string data, Game* game)
{
	game->m_running = false;
	return 0;
}

void* Game::processInventory(int id, std::string data, Game* game)
{
	game->m_sender.Send("TODO: Show inventory", game->m_connection_map[id]);
	return 0;
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

bool Game::isDirection(std::string input)
{
	std::string command = Tokenizer::GetFirstToken(input);
	command = Tokenizer::UpperCase(command);
	input.insert(0, command);
	return std::binary_search(DIRECTIONS.begin(), DIRECTIONS.end(), command);
}

std::string Game::createStatusBar(Player* player)
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
	return statusbar;
}

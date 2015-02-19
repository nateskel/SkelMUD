#include "Game.h"
#include "Tokenizer.h"
#include <iostream>
#include <algorithm>

Game::Game()
{
	m_running = false;
#ifdef _WIN_32
	m_mutex = CreateMutex(NULL, false, NULL);
#endif
	m_sender = Sender();
	m_output_manager = OutputManager();
	m_planets.push_back(BuildPlanet());
}

Game::~Game()
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
#ifdef _WIN32
	DWORD dwThreadId;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, ListenThread, this, 0, &dwThreadId);
#else
	pthread_t ptThreadID;
	int hThread;
	hThread = pthread_create(&ptThreadID, NULL, ListenThread, this);
#endif
	std::map<SOCKET, Connection*>::iterator it;
	std::map<SOCKET, Connection*>::iterator it_end;
	while (m_running)
	{
		//loop
		Thread::Lock(m_mutex);
		it = m_connection_map.begin();
		it_end = m_connection_map.end();
		Thread::Unlock(m_mutex);
		int DEBUGINT = 0;
		for (it; it != it_end; it++)
		{
			Thread::Lock(m_mutex);
			Connection* connection = it->second;
			if (connection == NULL)
				continue;
			Connection::State state = connection->GetState();
			Thread::Unlock(m_mutex);
			if (state == Connection::DISCONNECTED)
			{
				m_connection_map.erase(it);
				std::cout<< "ERASED" << std::endl;
				continue;
			}
			else
			{
				/*
				it++;
				if (it == it_end)
					break;
					*/
			}
			int id = it->first;
			if (state == Connection::CONNECTED)
			{
				std::cout << m_connection_map.size() << std::endl;
				m_sender.Send(m_output_manager.GetIntroText(), connection, YELLOW);
				connection->SetState(Connection::USERNAME);
				continue;
			}
			if (connection->HasMoreData())
			{
				std::string data = connection->GetNextData();
				RemoveEndline(data);
				switch(state)
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
		}
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
		std::string output = m_player_map[id]->GetName();
		output.append("(ooc): ");
		output.append(data);
		output.append("\r\n");
		m_sender.SendAll(output, m_connection_map, id, YELLOW);
	}
	else if (command == "HELP")
	{
		m_sender.Send("Commands are not case sensitive\r\n\r\nOOC <message>: Out of character global chat\r\nOOC <ON/OFF>: Turn OOC permanently ON/OFF\r\n", m_connection_map[id]);
	}
	else if (command == "QUIT1459")
	{
		m_running = false;
	}
	else if (command == "LOOK" || command == "L")
	{
		processLook(id);
	}
	else if (command == "N" || command == "S" || command == "E" || command == "W" ||
		command == "NORTH" || command == "SOUTH" || command == "EAST" || command == "WEST")
	{
		processDirectionCommand(command, id);
	}
	else if (command == "CON")
	{
		m_sender.Send(std::to_string(m_connection_map.size()), m_connection_map[id]);
	}
	else
	{
		m_sender.Send("Unrecognized Command. Type 'help' for a list of commands.\r\n", m_connection_map[id]);
	}
}

bool Game::processDirectionCommand(std::string command, int id)
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
	if (!result)
	{
		m_sender.Send("Can't go that way!\r\n", m_connection_map[id]);
	}
	else
	{
		std::vector<int> leaving_players = m_planets[current_planet]->GetRoom(current_room)->GetPlayerIDs(id);
		std::vector<int> arriving_players = m_planets[current_planet]->GetRoom(current_player->GetRoomID())->GetPlayerIDs(id);
		std::string arrive_message = current_player->GetName();
		arrive_message.append(" entered the room\r\n");
		std::string leave_message = current_player->GetName();
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
	std::vector<int> visible_players = room->GetPlayerIDs(id);
	std::string output = room->GetLongDescription();
	output.append("\r\n");
	output.append(room->GetShortDescription());
	output.append("\r\n\r\n");
	output.append(CYAN);
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

void Game::RemoveEndline(std::string &data)
{
	data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());
	data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
}

Planet* BuildPlanet()
{
	Planet* planet = new Planet();
	planet->AddRoom(new Room("This is the first room", "First Room", 1, -1, 3, -1, -1, -1, -1, -1, -1, -1));
	planet->AddRoom(new Room("This is the second room", "Second Room", -1, 0, 2, -1, -1, -1, -1, -1, -1, -1));
	planet->AddRoom(new Room("This is the third room", "Third Room", -1, 3, -1, 1, -1, -1, -1, -1, -1, -1));
	planet->AddRoom(new Room("This is the fourth room", "Fourth Room", 2, -1, -1, 0, -1, -1, -1, -1, -1, -1));
	return planet;
}

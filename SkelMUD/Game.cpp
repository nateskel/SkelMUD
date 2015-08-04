#include "Game.h"
#include "Tokenizer.h"
#include "File.h"
#include <iostream>
#include <algorithm>

#ifndef _WIN32

#include <unistd.h>
#include <sstream>

#define Sleep(seconds) sleep(seconds)
#endif

Game::Game() {
    // List of possible directional commands. Used to determine if entered command is a direction
    DIRECTIONS = {"NORTH", "N", "SOUTH", "S", "EAST", "E", "WEST", "W", "NORTHEAST", "NE", "NORTHWEST",
                  "NW", "SOUTHEAST", "SE", "SOUTHWEST", "SW", "UP", "U", "DOWN", "D"};
    // Presort the list for later use of binary_search
    std::sort(DIRECTIONS.begin(), DIRECTIONS.end());
    m_running = false;
#ifdef _WIN32
	m_mutex = CreateMutex(NULL, false, NULL);
#else
    pthread_mutex_init(&m_mutex, NULL);
#endif
    m_sender = Sender();
    m_output_manager = OutputManager();
    //m_planets.push_back(BuildPlanet());
    //File accountFile = File();
    File file = File();
    m_accounts = file.LoadAccounts();
    m_planets = file.LoadPlanets();
    registerCommands();
}

Game::~Game() {

}

void Game::registerCommands() {
    /*
     * In order to process a command entered by the user (or augmented through augmentCommand), it must be registered.
     * Register commands here using the RegisterCommand method. Usage:
     * *void RegisterCommand(std::string command_string, void *(*command_function)(int, std::string, Game *),
     *                       Connection::State state);
     * command_string: command to be processed (e.g. say)
     * command_function: function pointer to function to be called when command is issued (see below)
     * state: the Connection::State this command is valid for
     *
     * the command_function signature is as follows:
     * void *foo(int id, std::string data, Game *game)
     * id: the id of the connection to identify the particular telnet session
     * data: any tokens that follow the initial commands (e.g. say hello there - data will equal "hello there")
     * *game: a pointer to the Game object
     *
     * common use case in a command function:
     * to obtain a pointer to the connection that issued the command:
     * Connection* connection = game->m_connection_map[id];
     *
     * Currently, the same command can not be used more than once. A use case where this may be a problem is when
     * a command may have a different usage depending on the connection state. This use case does not currently exist,
     * but consideration of changing this architecture may be given if this becomes an issue
     *
     * TODO: Create logic to allow for Connection::None or no state parameter. Will ignore the state and process the
     * TODO: command. States with augmented commands will still ignore this command.
     */
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
    RegisterCommand("NEWUSER_CONFIRM", processNewUserConfirm, Connection::NEWUSER_CONFIRM);
    RegisterCommand("NEWPASSWORD", processNewPassword, Connection::NEWPASSWORD);
    RegisterCommand("CHARACTERSELECTION", processCharacterSelection, Connection::CHARACTER_SELECTION);
    RegisterCommand("TEST", processTest, Connection::LOGGEDIN);
    Logger::Debug("Registered Commands");
}

void Game::augmentCommand(Connection::State state, std::string &data) {
    /*
     * Connection states that request specific input from the user will not process commands entered by the user. Since
     * the command architecture requires a command to be processed, this function will insert this command into the
     * user's input so that it can be processed. This may also be used in other cases such as when the user inputs a
     * direction (e.g. "n" or "South").
     */
    if (state == Connection::USERNAME) {
        data.insert(0, "USERNAME ");
    }
    else if (state == Connection::PASSWORD) {
        data.insert(0, "PASSWORD ");
    }
    else if (state == Connection::NEWPASSWORD) {
        data.insert(0, "NEWPASSWORD ");
    }
    else if (state == Connection::OOC) {
        data.insert(0, "OOCON ");
    }
    else if (state == Connection::NEWUSER_CONFIRM) {
        data.insert(0, "NEWUSER_CONFIRM ");
    }
    else if (state == Connection::CHARACTER_SELECTION) {
        data.insert(0, "CHARACTERSELECTION ");
    }
    else if (isDirection(data)) {
        data = Tokenizer::UpperCase(data);
        data.insert(0, "DIRECTION ");
    }
}

void Game::Start() {
    m_running = true;
    T_HANDLE hThread = Thread::MakeThread(ListenThread, this);
    std::map<SOCKET, Connection*>::iterator it;
    std::map<SOCKET, Connection*>::iterator it_end;
    while (m_running) {
        //loop
        Thread::Lock(m_mutex);
        it = m_connection_map.begin();
        it_end = m_connection_map.end();
        Thread::Unlock(m_mutex);
        for (it; it != it_end;) {
            int id = it->first;
            Thread::Lock(m_mutex);
            Connection* connection = it->second;
            if (connection == NULL) {
                Thread::Unlock(m_mutex);
                continue;
            }
            Connection::State state = connection->GetState();
            Thread::Unlock(m_mutex);
            if (state == Connection::LOGGEDIN) {
                Player* player = m_player_map[id];
                if (player == NULL) {
                    Logger::Error("Logged in player not found!");
                }
                else {
                    std::string statusbar = createStatusBar(player);
                    connection->Send(statusbar);
                }
            }
            else
                connection->Send("\r\n> ");
            if (state == Connection::DISCONNECTED) {
                if (m_player_map.find(id) != m_player_map.end()) {
                    m_planets[m_player_map[id]->GetPlanetID()]->GetRoom(m_player_map[id]->GetRoomID())->RemovePlayer(
                            id);
                    delete m_player_map[id];
                    m_player_map.erase(id);
                }
                std::map<SOCKET, Connection*>::iterator it_temp = it;
                it++;
                m_connection_map.erase(it_temp);
                continue;
            }
            else if (state == Connection::CONNECTED) {
                Logger::Debug("User Connected");
                m_sender.Send(m_output_manager.GetIntroText(), connection, YELLOW);
                connection->SetState(Connection::USERNAME);
                it++;
                continue;
            }
            else {
                if (connection->HasMoreData()) {
                    std::string data = connection->GetNextData();
                    Utils::RemoveEndline(data);
                    augmentCommand(state, data);
//                    if(state == Connection::LOGGEDIN)
//                        m_sender.Send("\033[2A\033[K", connection);
                    if (!ProcessCommand(data, state, id)) {
                        m_sender.Send("Unrecognized Command. Type 'help' for a list of commands.\r\n", connection);
                    }
                    else {
                        m_sender.Send("", connection);
                    }
                }
                it++;
            }
        }
        Sleep(1);
    }
}

void Game::RegisterCommand(std::string command_string, void* (* command_function)(int, std::string, Game*),
                           Connection::State state) {
    Command command;
    command.command_function = command_function;
    command.state = state;
    m_command_map[command_string] = command;
}

bool Game::ProcessCommand(std::string command_string, Connection::State state, int id) {
    std::string command_input = Tokenizer::UpperCase(Tokenizer::GetFirstToken(command_string));
    if (command_input.size() == 0)
        return false;
    if (m_command_map.find(command_input) == m_command_map.end())
        return false;
    Command command = m_command_map[command_input];
    if (command.state == state) {
        Thread::Lock(m_mutex);
        command.command_function(id, command_string, this);
        Thread::Unlock(m_mutex);
        return true;
    }
    return false;
}

void Game::AddOutput(int id, std::string output) {
    std::map<int, std::string>::iterator iterator = m_output_map.find(id);
    if (iterator != m_output_map.end())
        iterator->second.append(output);
    else
        m_output_map[id] = output;
}

std::string Game::GetOutput(int id) {
    std::map<int, std::string>::iterator iterator = m_output_map.find(id);
    if (iterator != m_output_map.end())
        return iterator->second;
    else
        return NULL;
}

void* Game::processDirectionalCommand(int id, std::string command, Game* game) {
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
    if (!result) {
        game->m_sender.Send("Can't go that way!\r\n", game->m_connection_map[id]);
    }
    else {
        std::vector<int> leaving_players = game->m_planets[current_planet]->GetRoom(current_room)->GetPlayerIDs(id);
        std::vector<int> arriving_players = game->m_planets[current_planet]->GetRoom(
                current_player->GetRoomID())->GetPlayerIDs(id);
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

void* Game::processLook(int id, std::string data, Game* game) {
    Player* current_player = game->m_player_map[id];
    Room* room = game->m_planets[current_player->GetPlanetID()]->GetRoom(current_player->GetRoomID());
    std::string output = room->GetLongDescription();
    output.append("\r\n");
    output.append(room->GetShortDescription());
    output.append("\r\n\r\n");
    output.append(CYAN);

    std::vector<int> visible_players = room->GetPlayerIDs(id);
    for (int i = 0; i < (int) visible_players.size(); i++) {
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

void* Game::processUsername(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    Utils::RemoveEndline(data);
    std::string username = "";
    std::vector<std::string> connected_users;
    for (auto i = game->m_connection_map.begin(); i != game->m_connection_map.end(); ++i) {
        connected_users.push_back(i->second->GetAccount().username);
    }
    if (std::find(connected_users.begin(), connected_users.end(), data) != connected_users.end()) {
        game->m_sender.Send("Player already connected!\r\nEnter Username:", connection);
        return 0;
    }
    auto pred = [data](const Connection::Account &item) {
        return item.username == data;
    };
    auto account = std::find_if(std::begin(game->m_accounts), std::end(game->m_accounts), pred);
    if (account != std::end(game->m_accounts)) {
        username = (*account).username;
        //TODO: This will be changed to character creation
//        Player* player = new Player(id, data);
//        game->m_player_map[id] = player;
//        game->m_planets[player->GetPlanetID()]->GetRoom(player->GetRoomID())->AddPlayer(player);
        game->m_sender.Send("Password: ", connection, YELLOW);
        connection->SetState(Connection::PASSWORD);
    }
    else {
        game->m_sender.Send("Account does not exist, create new account? (Y/N)", connection);
        Connection::Account new_account;
        new_account.username = data;
        new_account.id = -1;
        new_account.password = "";
        connection->SetAccount(new_account);
        connection->SetState(Connection::NEWUSER_CONFIRM);
    }
    return 0;
}

void* Game::processPassword(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    game->m_sender.Send("Logged in!\r\n", connection);
    game->m_sender.Send("Select Character:\r\n\r\n", connection);
    //TODO: temporary placeholder
    game->m_sender.Send("None\r\n\r\n", connection);
    game->m_sender.Send("[0] Create New Character", connection);
    connection->SetState(Connection::CHARACTER_SELECTION);
    //connection->SetState(Connection::LOGGEDIN);
    //processLook(id, "", game);
    return 0;
}

void* Game::processSay(int id, std::string data, Game* game) {
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

void* Game::processOOC(int id, std::string data, Game* game) {
    std::vector<std::string> commandlist = Tokenizer::GetAllTokens(data);
    if (commandlist.size() == 0)
        return 0;
    if (commandlist.size() == 2 && commandlist[0] == "OOC" && Tokenizer::UpperCase(commandlist[1]) == "OFF") {
        game->m_sender.Send("OOC Off\r\n", game->m_connection_map[id], MAGENTA);
        game->m_connection_map[id]->SetState(Connection::LOGGEDIN);
        return 0;
    }
    if (Tokenizer::UpperCase(commandlist[0]) == "ON") {
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

void* Game::processHelp(int id, std::string data, Game* game) {
    game->m_sender.Send(
            "Commands are not case sensitive\r\n\r\nOOC <message>: Out of character global chat\r\nOOC <ON/OFF>: Turn OOC permanently ON/OFF\r\nSAY <message>: In character local chat.\r\n",
            game->m_connection_map[id]);
    return 0;
}

void* Game::processQuit(int id, std::string data, Game* game) {
    game->m_running = false;
    return 0;
}

void* Game::processInventory(int id, std::string data, Game* game) {
    game->m_sender.Send("TODO: Show inventory", game->m_connection_map[id]);
    return 0;
}

THREAD Game::ListenThread(LPVOID lpParam) {
    Game* game = (Game*) lpParam;

    ServerSocket myListener = ServerSocket(25570); // 25570
    myListener.Initialize();
    int error = myListener.Listen();
    while (game->m_running) {
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

bool Game::isDirection(std::string input) {
    std::string command = Tokenizer::GetFirstToken(input);
    command = Tokenizer::UpperCase(command);
    input.insert(0, command);
    return std::binary_search(DIRECTIONS.begin(), DIRECTIONS.end(), command);
}

std::string Game::createStatusBar(Player* player) {
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

void* Game::processNewUser(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    std::string username = Tokenizer::GetFirstToken(data);
    auto pred = [username](const Connection::Account &item) {
        return item.username == username;
    };
    if (std::find_if(std::begin(game->m_accounts), std::end(game->m_accounts), pred) != std::end(game->m_accounts)) {
        game->m_sender.Send("That username already exists!", connection);
    }
    else {
        game->m_sender.Send("Password: ", connection);
        connection->SetState(Connection::NEWPASSWORD);
    }
    return 0;
}

void* Game::processNewUserConfirm(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    Utils::RemoveEndline(data);
    data = Tokenizer::UpperCase(data);
    if (data == "Y" || data == "YES") {
        game->m_sender.Send("Password: ", connection);
        connection->SetState(Connection::NEWPASSWORD);
    }
    else {
        game->m_sender.Send("Enter Username: ", connection);
        connection->SetState(Connection::USERNAME);
    }
    return 0;
}

void* Game::processNewPassword(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    Utils::RemoveEndline(data);
    Connection::Account account = connection->GetAccount();
    account.password = data;
    account.id = game->m_accounts.size();
    account.level = Connection::Standard;
    std::fstream file_stream;
    File file = File();
    file.SaveAccount(file_stream, account);
    game->m_accounts.push_back(account);
    game->m_sender.Send("Logged in!\r\n", connection);
    game->m_sender.Send("Select Character:", connection);
    //TODO: temporary placeholder
    game->m_sender.Send("None\r\n", connection);
    game->m_sender.Send("[0] Create New Character", connection);
    connection->SetState(Connection::CHARACTER_SELECTION);
    //connection->SetState(Connection::LOGGEDIN);
    //game->m_player_map[id] = new Player(id, account.username);
    //game->m_planets[game->m_player_map[id]->GetPlanetID()]->GetRoom(game->m_player_map[id]->GetRoomID())->AddPlayer(game->m_player_map[id]);
    //processLook(id, "", game);
    return 0;
}

void* Game::processCharacterSelection(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    //TODO: error checking and character selection logic
    int selection = std::atoi(data.c_str());
    if (selection == 0) {
        //TODO: Handle Character Creation
        game->m_player_map[id] = new Player(id, connection->GetAccount().username);
        Logger::Debug(connection->GetAccount().username);
        game->m_planets[game->m_player_map[id]->GetPlanetID()]->GetRoom(game->m_player_map[id]->GetRoomID())->AddPlayer(
                game->m_player_map[id]);
        processLook(id, "", game);
        connection->SetState(Connection::LOGGEDIN);
    }
    else {
        // Handle Character Selection
    }
    return 0;
}

void* Game::processTest(int id, std::string data, Game* game) {
    Connection* connection = game->m_connection_map[id];
    Player* player = game->m_player_map[id];
    player->SetHP(player->GetHP() - 5);
    game->m_sender.Send("\033[2A\033[K", connection);
    return 0;
}

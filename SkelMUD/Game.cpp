#include "Game.h"
#include "Sender.h"
#include "States/StateFactory.h"
#include "Format.h"
#include <iostream>
#include <algorithm>
#include <thread>

#ifndef _WIN32
#include <mutex>
#include <random>
#include <cmath>

#endif

Game::Game() {
    isRunning = true;
    Logger::Debug("Setting up GameData");
    m_game_data = std::make_shared<GameData>();
    connection_id = 0;
    main_elapsed = std::time(nullptr);
    regen_elapsed = std::time(nullptr);
    Logger::Debug("Initializing States");
    port = 4321;
}

std::mutex Game::game_mutex;

Game::~Game() {

}

void Game::Start(int port) {
    m_game_data->GetConfiguration().SetPort(port);
    Start();
}

void Game::Start() {
    std::thread listener(&Game::listenerThread, this);
    listener.detach();
    std::list<int> disconnected;
    Logger::Info("Running\n");
    while(isRunning) {
        std::map<int, std::shared_ptr<Connection>> connection_map = m_game_data->GetAllConnections();
        for(auto connection_map_entry : connection_map)
        {
            std::lock_guard<std::mutex> lock(Game::game_mutex);
            std::shared_ptr<Connection> connection = connection_map_entry.second;
            if(!connection->IsConnected())
            {
                if(connection->IsLoggedIn()) {
                    auto player = m_game_data->GetPlayer(connection->GetCharacterName());
                    m_game_data->GetRoom(player->GetLocationID(),
                                         player->GetRoomID(),
                                         player->IsInShip())->RemovePlayer(player->GetID());
                }
                m_game_data->EraseConnection(connection->GetID());
                std::stringstream ss;
                ss << connection->GetIP() << " has disconnected (connection dropped)";
                Logger::Info(ss.str());
                continue;
            }
            if(connection->IsPromptTick()) {
                Sender::UpdatePrompt(connection);
            }
            std::string received = connection->GetNextReceived();
            Utils::RemoveEndline(received);
            if(connection->IsLoggedIn()) {
                std::string data = connection->GetPlayer()->GetCommData();
                if(data != "")
                    Sender::Send(data, connection);
            }
            connection->FlushOutput();
            if(received == "")
                continue;
            std::stringstream ss;
            ss << "Received data (" << connection->GetIP() << "): " << received;
            Logger::Debug(ss.str());
            connection->GetState()->processInput(received, connection);
            if(connection->IsStateChanged())
            {
                connection->GetState()->init(connection);
                connection->ResetStateChanged();
            }
        }
        if(std::time(nullptr) - main_elapsed >= 3) {
            ProcessShips();
            ProcessCombat();
            main_elapsed = std::time(nullptr);
        }
        if(std::time(nullptr) - regen_elapsed >= 5) {
            ProcessRegen();
            regen_elapsed = std::time(nullptr);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::ProcessRegen() {
    for(auto item: m_game_data->GetPlayers()) {
        auto player = item.second;
        player->Regen(player->IsFighting());
    }
}

void Game::ProcessCombat() {
    for(auto item: m_game_data->GetPlayers()) {
        auto player = item.second;
        if(player->IsFighting()) {
            auto target = player->GetTarget();
            std::stringstream ss;
            std::stringstream ts;
            auto main_hand = player->GetMainHand();
            auto off_hand = player->GetOffHand();
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> crit_dist(1, 10);
            if(main_hand != nullptr) {
                bool crit = crit_dist(mt) > 9;
                auto weapon = std::dynamic_pointer_cast<Wieldable>(main_hand->GetMixin("Wieldable"));
                std::uniform_int_distribution<int> dist(weapon->GetMinDamage(), weapon->GetMaxDamage());
                int damage = dist(mt);
                auto weapon_scale = weapon->GetScale();
                damage = int(round((float)damage * GetScaledModifier(player, weapon_scale)));
                std::string damage_text;
                std::string damage_text_other;
                if(!crit) {
                    damage_text = weapon->GetNormalHit();
                    damage_text_other = weapon->GetNormalHitOther();
                }
                else {
                    damage_text = weapon->GetCriticalHit();
                    damage_text_other = weapon->GetCriticalHitOther();
                    damage *= 3;
                }
                ss << "You " << damage_text << " " << target->GetName();
                ss << " for " << damage << " damage!" << std::endl;
                ts << player->GetName() << " " << damage_text_other;
                ts << " you for " << damage << " damage!" <<std::endl;
                target->Damage(damage);
            }
            if(off_hand != nullptr) {
                bool crit = crit_dist(mt) > 9;
                auto weapon = std::dynamic_pointer_cast<Wieldable>(off_hand->GetMixin("Wieldable"));
                std::uniform_int_distribution<int> dist(weapon->GetMinDamage(), weapon->GetMaxDamage());
                int damage = dist(mt);
                std::string damage_text;
                std::string damage_text_other;
                if(!crit) {
                    damage_text = weapon->GetNormalHit();
                    damage_text_other = weapon->GetNormalHitOther();
                }
                else {
                    damage_text = weapon->GetCriticalHit();
                    damage_text_other = weapon->GetCriticalHitOther();
                    damage *= 3;
                }
                ss << "You " << damage_text << " " << target->GetName();
                ss << " for " << damage << " damage!" << std::endl;
                ts << player->GetName() << " " << damage_text_other;
                ts << " you for " << damage << " damage!" <<std::endl;
                target->Damage(damage);
            }
            if(main_hand == nullptr && off_hand == nullptr) {
                ss << "You punch " << target->GetName() << " for 1 damage!" << std::endl;
                ts << player->GetName() << " punches you for 1 damage!" << std::endl;
                target->Damage(1);
            }
            player->Send(ss.str());
            target->Send(ts.str());
        }
    }
}

void Game::ProcessSkill(std::shared_ptr<Skill> skill) {

}

void Game::ProcessShips() {
    for(auto ship : m_game_data->GetShips().GetShips()) {
        if(!ship.second->IsInOrbit()) {
            Utils::Vector3 coords = ship.second->GetCoordinates();
            Utils::Vector3 velocity = ship.second->GetVelocity();
            double x = coords.x + velocity.x;
            double y = coords.y + velocity.y;
            double z = coords.z + velocity.z;
            ship.second->SetCoordinates(x, y, z);
            bool is_moving = (velocity.x + velocity.y + velocity.z) != 0;
            double speed = Utils::GetDistance(0, 0, 0, velocity);
            if(ship.second->IsInSpace() and
               is_moving and
               Utils::GetDistance(ship.second->GetCoordinates(),
                                  ship.second->GetDestination()) <= speed * 1.3) {
                if(speed > 10) {
                    Utils::Vector3 new_velocity = ship.second->GetVelocity();
                    new_velocity.x /= 10;
                    new_velocity.y /= 10;
                    new_velocity.z /= 10;
                    ship.second->SetVelocity(new_velocity);
                    std::stringstream ss;
                    ss << "Approaching Destination, slowing speed to " << speed / 10 << "\n";
                    Sender::SendToMultiple(ss.str(),
                                           m_game_data->GetLoggedInConnections(),
                                           ship.second->GetPlayerIDs());
                }
                else {
                    Sender::SendToMultiple("Arrived at destination\n",
                                           m_game_data->GetLoggedInConnections(),
                                           ship.second->GetPlayerIDs());
                    ship.second->SetVelocity(0, 0, 0);
                }
            }
        }
    }
}

void Game::listenerThread() {
    ServerSocket socket(m_game_data->GetConfiguration().GetPort());
    socket.Listen();
    while(isRunning) {
        DataSocket dataSocket = socket.Accept();
        auto connection = std::make_shared<Connection>(dataSocket);
        connection->SetState(GameStates::LOGIN, m_game_data);
        connection->GetState()->init(connection);
        connection->ResetStateChanged();
        std::lock_guard<std::mutex> guard(Game::game_mutex);
        // TODO: connection_id temporary for debugging
        // TODO: eventually connection_id could increment beyond the size of int
        connection->SetID(connection->GetSocket());
        ++connection_id;
        m_game_data->AddConnection(connection);
        connection->Run();
    }
}

float Game::GetScaledModifier(const std::shared_ptr<Player> &player, ScaleAttribute scaled) {
    float result = 0;
    switch(scaled) {
        case STR:
            result = (float)player->GetNetStrength() / 10.0f;
            break;
        case DEX:
            result = (float)player->GetNetDexterity() / 10.0f;
            break;
        case END:
            result = (float)player->GetNetEndurance() / 10.0f;
            break;
        case INT:
            result = (float)player->GetNetIntelligence() / 10.0f;
            break;
        case SKILL:
            result = (float)player->GetNetSkill() / 10.0f;
            break;
    }
    return result;
}

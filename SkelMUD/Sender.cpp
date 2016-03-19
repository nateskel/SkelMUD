#include "Sender.h"
#include "Format.h"

void Sender::SendAll(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, int exclude) {
    SendAll(data, connection_map, exclude, "");
}

void Sender::SendAll(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, int exclude,
                     std::string color) {
    data.insert(0, color);
    data.append(Format::RESET);
    for(auto connection_entry : connection_map)
    {
        if(connection_entry.first != exclude)
            connection_entry.second->AddOutput(data + connection_entry.second->GetPrompt());
    }
}

void Sender::SendTo(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, int target) {
    SendTo(data, connection_map, target, "");
}

void Sender::SendTo(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, int target, std::string color) {
    data.insert(0, color);
    data.append(Format::RESET);
    std::map<SOCKET, std::shared_ptr<Connection>>::iterator it;
    for (it = connection_map.begin(); it != connection_map.end(); it++) {
        if (it->second->GetID() == target) {
            it->second->AddOutput(data);
            break;
        }
    }
}

void Sender::Send(std::string data, std::shared_ptr<Connection> connection) {
    Send(data, connection, "");
}

void Sender::Send(std::string data, std::shared_ptr<Connection> connection, std::string color) {
    data.insert(0, color);
    data.append(Format::RESET);
    connection->AddOutput(data);
}

void Sender::SendToMultiple(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, std::vector<int> targets) {
    SendToMultiple(data, connection_map, targets, "");
}

void Sender::SendToMultiple(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, std::vector<int> targets,
                            std::string color) {
    data.insert(0, color);
    data.append(Format::RESET);
    std::map<SOCKET, std::shared_ptr<Connection>>::iterator it;
    for (u_int i = 0; i < targets.size(); i++) {
        for (it = connection_map.begin(); it != connection_map.end(); it++)
            if (targets[i] == it->first) {
                it->second->AddOutput(data);
                break;
            }
    }
}

void Sender::SendAll(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, std::vector<int> exclude) {
    SendAll(data, connection_map, exclude, "");
}

void Sender::SendAll(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map, std::vector<int> exclude,
                     std::string color) {
    data.insert(0, color);
    data.append(Format::RESET);
    for(auto connection_entry : connection_map)
    {
        for(int ex : exclude) {
            if (connection_entry.first != ex)
                connection_entry.second->AddOutput(data);
        }
    }
}

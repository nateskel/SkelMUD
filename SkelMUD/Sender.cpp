#include "Sender.h"

Sender::Sender() {

}

Sender::~Sender() {

}

void Sender::SendAll(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET exclude) {
    SendAll(data, connection_map, exclude, "");
}

void Sender::SendAll(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET exclude,
                     std::string color) {
    data.insert(0, color);
    data.append(RESET);
    std::map<SOCKET, Connection*>::iterator it;
    for (it = connection_map.begin(); it != connection_map.end(); it++) {
        if (it->second == NULL)
            continue;
        if (it->second->GetSocket() != exclude &&
            (it->second->GetState() == Connection::LOGGEDIN || it->second->GetState() == Connection::OOC)) {
            it->second->StageSend(data);
        }
    }
}

void Sender::SendTo(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET target) {
    SendTo(data, connection_map, target, "");
}

void Sender::SendTo(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET target, std::string color) {
    data.insert(0, color);
    data.append(RESET);
    std::map<SOCKET, Connection*>::iterator it;
    for (it = connection_map.begin(); it != connection_map.end(); it++) {
        if (it->second->GetSocket() == target) {
            it->second->StageSend(data);
            break;
        }
    }
}

void Sender::Send(std::string data, Connection* connection) {
    Send(data, connection, "");
}

void Sender::Send(std::string data, Connection* connection, std::string color) {
    data.insert(0, color);
    data.append(RESET);
    connection->StageSend(data);
}

void Sender::SendToMultiple(std::string data, std::map<SOCKET, Connection*> connection_map, std::vector<int> targets) {
    SendToMultiple(data, connection_map, targets, "");
}

void Sender::SendToMultiple(std::string data, std::map<SOCKET, Connection*> connection_map, std::vector<int> targets,
                            std::string color) {
    data.insert(0, color);
    data.append(RESET);
    std::map<SOCKET, Connection*>::iterator it;
    for (u_int i = 0; i < targets.size(); i++) {
        for (it = connection_map.begin(); it != connection_map.end(); it++)
            if (targets[i] == it->first) {
                it->second->StageSend(data);
                break;
            }
    }
}
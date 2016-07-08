#ifndef SENDER_H
#define SENDER_H

#include "Connection.h"
#include "Network.h"
#include <map>
#include <vector>
#include <string>

class Sender {
public:
//    Sender();
//
//    ~Sender();

    static void SendAll(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map, SOCKET exclude);

    static void SendAll(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map,
                        std::vector<int> exclude);

    static void SendAll(std::string data, std::map<int, std::shared_ptr<Connection>> connection_map,
                        std::vector<int> exclude,
                        std::string color);

    static void SendTo(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map, SOCKET target);

    static void Send(std::string data, std::shared_ptr<Connection> connection);

    static void SendToMultiple(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map,
                               std::vector<int> targets);

    static void SendAll(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map, SOCKET exclude,
                        std::string color);

    static void SendTo(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map, SOCKET target,
                       std::string color);

    static void Send(std::string data, std::shared_ptr<Connection> connection, std::string color);

    static void SendToMultiple(std::string data, std::map<SOCKET, std::shared_ptr<Connection>> connection_map,
                               std::vector<int> targets,
                               std::string color);

    static void UpdatePrompt(std::shared_ptr<Connection> connection);

protected:
private:
};

#endif // SENDER_H

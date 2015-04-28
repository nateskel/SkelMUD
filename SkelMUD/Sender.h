#ifndef SENDER_H
#define SENDER_H

#include "Connection.h"
#include <map>
#include <vector>
#include <string>

static const std::string NONE = "";
static const std::string RESET = "\33[0m";
static const std::string BOLD = "\33[1m";
static const std::string NORMAL = "\33[22m";
static const std::string BLACK = "\33[30m";
static const std::string RED = "\33[31m";
static const std::string GREEN = "\33[32m";
static const std::string YELLOW = "\33[33m";
static const std::string BLUE = "\33[34m";
static const std::string MAGENTA = "\33[35m";
static const std::string CYAN = "\33[36m";
static const std::string WHITE = "\33[37m";

class Sender {
public:
    Sender();

    ~Sender();

    void SendAll(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET exclude);

    void SendTo(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET target);

    void Send(std::string data, Connection* connection);

    void SendToMultiple(std::string data, std::map<SOCKET, Connection*> connection_map, std::vector<int> targets);

    void SendAll(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET exclude, std::string color);

    void SendTo(std::string data, std::map<SOCKET, Connection*> connection_map, SOCKET target, std::string color);

    void Send(std::string data, Connection* connection, std::string color);

    void SendToMultiple(std::string data, std::map<SOCKET, Connection*> connection_map, std::vector<int> targets,
                        std::string color);

protected:
private:
    //std::map<SOCKET, Connection*> m_connection_map;
};

#endif // SENDER_H

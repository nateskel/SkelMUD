#ifndef SENDER_H
#define SENDER_H

#include "Connection.h"
#include "Network.h"
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

protected:
private:
    //std::map<SOCKET, Connection*> m_connection_map;
};

#endif // SENDER_H

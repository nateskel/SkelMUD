//
// Created by nate on 11/13/15.
//

#ifndef SKELMUD_CONNECTION_H
#define SKELMUD_CONNECTION_H

#include <string>
#include <list>
#include "Network.h"

class Connection {
public:
    Connection(DataSocket &dataSocket) : dataSocket(dataSocket) { }

private:
    void connectionThread();
    DataSocket dataSocket;
    bool isRunning;
    std::string m_send_buffer;
    std::list<std::string> m_receive_buffer;

public:
    void Run();
    void Send(char* output);
    void AddOutput(std::string output);
    void FlushOutput();
    void Close();
    SOCKET GetSocket();
    std::string GetNextReceived();
};

#endif //SKELMUD_CONNECTION_H

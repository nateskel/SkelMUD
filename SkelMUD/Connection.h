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
    bool is_connected;
    std::string m_send_buffer;
    std::string state;
    std::list<std::string> m_receive_buffer;
    std::string owner_ip;

public:
    Connection();
    void Run();
    void Send(char* output);
    void AddOutput(std::string output);
    void FlushOutput();
    void Close();
    void SetState(std::string connection_state);
    std::string GetState();
    SOCKET GetSocket();
    std::string GetNextReceived();
    std::string GetIP();
    bool IsConnected();
};

#endif //SKELMUD_CONNECTION_H

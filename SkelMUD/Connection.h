//
// Created by nate on 11/13/15.
//

#ifndef SKELMUD_CONNECTION_H
#define SKELMUD_CONNECTION_H

#include <string>
#include <list>
#include "Network.h"
#include "Account.h"

class Connection {
public:
    Connection(DataSocket &dataSocket) : dataSocket(dataSocket), id(-1) { }

private:
    void connectionThread();
    DataSocket dataSocket;
    bool is_connected;
    bool state_changed;
    int id;
    std::string m_send_buffer;
    std::string state;
    std::list<std::string> m_receive_buffer;
    std::string owner_ip;
    Account account;
    std::string username;
    std::string password;

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
    bool IsStateChanged();
    void ResetStateChanged();
    int GetID();
    void SetID(int connection_id);
    Account GetAccount();
    std::string GetUsername();
    std::string GetPassword();
    void SetAccount(Account account);
    void SetUsername(std::string username);
    void SetPassword(std::string password);
    std::string GetPrompt();
};

#endif //SKELMUD_CONNECTION_H

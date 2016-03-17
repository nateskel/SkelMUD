//
// Created by nate on 11/13/15.
//

#include <thread>
#include <vector>
#include <list>
#include <sstream>
#include "Connection.h"
#include "Logger.h"
#include "Tokenizer.h"

void Connection::Run() {
    std::thread connThread(&Connection::connectionThread, this);
    is_connected = true;
    owner_ip = dataSocket.GetIP();
    std::stringstream ss;
    ss << "Incoming connection from " << owner_ip;
    Logger::Debug(ss.str());
    connThread.detach();
}

void Connection::connectionThread() {
    char buffer[1000];
    while(is_connected) {
        memset(buffer, 0, sizeof(buffer));
        int result = dataSocket.Receive(buffer);
        Logger::Debug("Receive result: " + std::to_string(result));
        if(result <= 0) {
            is_connected = false;
            perror("Error: ");
        }
        else {
            std::string string_data = std::string(buffer);
            m_receive_buffer.push_back(string_data);
        }
    }
    Close();
}

void Connection::Send(char* output) {
    dataSocket.Send(output);
}

void Connection::AddOutput(std::string output) {
    m_send_buffer.append(output);
}

void Connection::FlushOutput() {
    if (m_send_buffer == "")
        return;
    m_send_buffer.append("\r\n");
    std::vector<char> output(m_send_buffer.begin(), m_send_buffer.end());
    output.push_back('\0');
    int sent = dataSocket.Send(&output[0]);
    if (sent == -1) {
        is_connected = false;
    }
    m_send_buffer = "";
}

void Connection::Close() {
    dataSocket.Close();
}

std::string Connection::GetNextReceived() {
    if(m_receive_buffer.size() == 0)
        return "";
    std::string output = m_receive_buffer.front();
    m_receive_buffer.pop_front();
    return output;
}

SOCKET Connection::GetSocket() {
    return dataSocket.GetSocket();
}

Connection::Connection() {
    state = "";
}

void Connection::SetState(std::string connection_state) {
    state = connection_state;
}

std::string Connection::GetState() {
    return state;
}

std::string Connection::GetIP() {
    return owner_ip;
}

bool Connection::IsConnected() {
    return is_connected;
}

int Connection::GetID() {
    return id;
}

void Connection::SetID(int connection_id) {
    id = connection_id;
}

std::string Connection::GetUsername() {
    return username;
}

std::string Connection::GetPassword() {
    return password;
}

void Connection::SetAccount(Account account) {
    this->account = account;
}

void Connection::SetUsername(std::string username) {
    this->username = username;
}

void Connection::SetPassword(std::string password) {
    this->password = password;
}

Account Connection::GetAccount() {
    return account;
}

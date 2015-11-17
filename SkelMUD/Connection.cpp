//
// Created by nate on 11/13/15.
//

#include <thread>
#include <vector>
#include <list>
#include "Connection.h"
#include "Logger.h"
#include "Tokenizer.h"

void Connection::Run() {
    std::thread connThread(&Connection::connectionThread, this);
    isRunning = true;
    Logger::Debug("Connection Started\n");
    connThread.detach();
}

void Connection::connectionThread() {
    char buffer[1000];
    dataSocket.Send("CONNECTED\n");
    while(isRunning) {
        memset(buffer, 0, sizeof(buffer));
        int result = dataSocket.Receive(buffer);
        if(result == 0)
            isRunning = false;
        else {
            std::string string_data = std::string(buffer);
            m_receive_buffer.push_back(string_data);
        }
    }
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
    std::vector<char> output(m_send_buffer.begin(), m_send_buffer.end());
    output.push_back('\0');
    int sent = dataSocket.Send(&output[0]);
    if (sent == -1) {
//        SetState(DISCONNECTED);
        isRunning = false;
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

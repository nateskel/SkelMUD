#include "Network.h"
#include <iostream>
#include <unistd.h>

ServerSocket::ServerSocket(int port) {
    m_listening = false;
    m_port = (u_int16_t)port;
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

ServerSocket::~ServerSocket() {
    //TODO: Garbage collection
}

int ServerSocket::Initialize() {
    return 0;
}

int ServerSocket::Listen() {
    m_listening = true;
    m_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int opt_val = 1;
    int opt_val_length = sizeof(opt_val);
    setsockopt(m_listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt_val, opt_val_length);

    sockaddr_in socket_address;

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(m_port);
#ifdef _WIN32
        socket_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    #else
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
    memset(&(socket_address.sin_zero), 0, 8);

    bind(m_listen_socket, (sockaddr*) &socket_address, sizeof(sockaddr));

    int error = listen(m_listen_socket, 16);
    return error;
}

DataSocket ServerSocket::Accept() {

    sockaddr_in socket_address;

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(m_port);
#ifdef _WIN32
        socket_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    #else
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
    memset(&(socket_address.sin_zero), 0, 8);

    SOCKET data_socket;
    socklen_t socket_address_size = sizeof(sockaddr_in);
    data_socket = accept(m_listen_socket, (sockaddr*) &socket_address, &socket_address_size);
    char buff[20];
    memset(&buff, 0, sizeof(buff));
    DataSocket dataSocket(data_socket);
    dataSocket.SetIP(inet_ntop(AF_INET, &socket_address.sin_addr, buff, INET_ADDRSTRLEN));
    return dataSocket;
}

int ServerSocket::Close() {
#ifdef _WIN32
	closesocket(m_listen_socket);
	#endif
    return 0;
}

DataSocket::DataSocket() {
    m_data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

DataSocket::~DataSocket() {

}

DataSocket::DataSocket(SOCKET socket) {
    m_data_socket = socket;
}

int DataSocket::Connect() {
    return 0;
}

int DataSocket::Send(char* data) {
    int sent = (int) send(m_data_socket, data, strlen(data) + 1, MSG_NOSIGNAL);
    int err = errno;
    return sent;
}

int DataSocket::Receive(char* data) {
    return (int) recv(m_data_socket, data, 1000, 0);
}

SOCKET DataSocket::GetSocket() {
    return m_data_socket;
}

void DataSocket::Close() {
#ifdef WIN32
    closesocket(m_data_socket);
#else
    shutdown(m_data_socket, SHUT_RDWR);
    close(m_data_socket);
#endif
}

std::string DataSocket::GetIP() {
    return ip;
}

void DataSocket::SetIP(std::string remote_ip) {
    ip = remote_ip;
}

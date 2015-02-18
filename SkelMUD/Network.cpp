#include "Network.h"
#include <iostream>

ServerSocket::ServerSocket(int port)
{
	m_listening = false;
	m_port = port;
	#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #endif
}

ServerSocket::~ServerSocket()
{
	//TODO: Garbage collection
}

int ServerSocket::Initialize()
{
	return 0;
}

int ServerSocket::Listen()
{
	m_listening = true;
	m_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in socket_address;

	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(m_port);
	#ifdef _WIN32
        socket_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    #else
        socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    #endif
	memset(&(socket_address.sin_zero), 0, 8);

	bind(m_listen_socket, (sockaddr*)&socket_address, sizeof(sockaddr));

	int error = listen(m_listen_socket, 16);
	return error;
}

DataSocket* ServerSocket::Accept()
{

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
	data_socket = accept(m_listen_socket, (sockaddr*)&socket_address, &socket_address_size);
	DataSocket* dataSocket = new DataSocket(data_socket);
	return dataSocket;
}

int ServerSocket::Close()
{
	#ifdef _WIN32
	closesocket(m_listen_socket);
	#endif
	return 0;
}

DataSocket::DataSocket()
{
	m_data_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

DataSocket::~DataSocket()
{

}

DataSocket::DataSocket(SOCKET socket)
{
	m_data_socket = socket;
}

int DataSocket::Connect()
{
	return 0;
}

int DataSocket::Send(char* data)
{
	send(m_data_socket, data, strlen(data) + 1, 0);
	return 0;
}

void DataSocket::Receive(char* data)
{
    recv(m_data_socket, data, 1000, 0);
}

SOCKET DataSocket::GetSocket()
{
	return m_data_socket;
}

void DataSocket::Close()
{
	#ifdef WIN32
    closesocket(m_data_socket);
    #endif
}

#ifndef	NETWORK_H
#define NETWORK_H

#ifdef _WIN32
	#include <ws2tcpip.h>
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/un.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #define SOCKET int
	#define closesocket shutdown
#endif

class DataSocket
{
public:
	DataSocket();
	DataSocket(SOCKET socket);
	~DataSocket();

	int Connect();
	int Send(char* data);
	int Receive(char* data);
	void Close();
	SOCKET GetSocket();

private:
	SOCKET m_data_socket;

};

class ServerSocket
{
public:

	ServerSocket(int port);
	~ServerSocket();

	int Listen();
	int Close();
	int Initialize();
	DataSocket* Accept();

private:
	int m_port;
	bool m_listening;
	SOCKET m_listen_socket;

	// private default constructor
	ServerSocket() { }
};

#endif

#include "Connection.h"
#include <iostream>

Connection::Connection(DataSocket* socket)
{
	m_socket = socket;
	m_mutex = CreateMutex(NULL, false, NULL);
	m_state = CONNECTED;
}

Connection::Connection()
{

}

Connection::~Connection()
{
	delete m_socket;
	m_socket = NULL;
	m_running = false;
}

void Connection::Run()
{
    #ifdef _WIN32
        DWORD dwThreadId;
        HANDLE hThread;
        hThread = CreateThread(NULL, 0, ConnectionThread, this, 0, &dwThreadId);
    #else
        pthread_t ptThreadID;
        int hThread;
        hThread = pthread_create( &ptThreadID, NULL, ConnectionThread, this);
    #endif
    m_running = true;
}

bool Connection::IsRunning()
{
	return m_running;
}

void Connection::Send(char* data)
{
	m_socket->Send(data);
}

void Connection::Receive(char* data)
{
    m_socket->Receive(data);
}

void Connection::Stop()
{
	m_running = false;
}

void Connection::Close()
{
    m_socket->Close();
}

HANDLE Connection::GetMutex()
{
	return m_mutex;
}

void Connection::SetState(State state)
{
	m_state = state;
}

Connection::State Connection::GetState()
{
	return m_state;
}

SOCKET Connection::GetSocket()
{
	return m_socket->GetSocket();
}

void Connection::AddReceivedData(std::string data)
{
	WaitForSingleObject(m_mutex, INFINITE);
	m_receive_list.push_back(data);
	ReleaseMutex(m_mutex);
}

std::string Connection::GetNextData()
{
	WaitForSingleObject(m_mutex, INFINITE);
	std::string output = m_receive_list.front();
	m_receive_list.pop_front();
	ReleaseMutex(m_mutex);
	return output;
}

bool Connection::HasMoreData()
{
	WaitForSingleObject(m_mutex, INFINITE);
	bool result = m_receive_list.size() > 0;
	ReleaseMutex(m_mutex);
	return result;
}

THREAD Connection::ConnectionThread(LPVOID lpParam)
{
	Connection* connection = (Connection*)lpParam;
	char output[1000];
	while (connection->IsRunning())
	{
		memset(output, 0, sizeof(output));
		connection->Receive(output);
		if (std::string(output) != "\r\n")
			connection->AddReceivedData(std::string(output));
	}
	connection->Close();
	return 0;
}

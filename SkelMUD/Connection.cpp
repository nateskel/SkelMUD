#include "Connection.h"
#include <iostream>
#include <vector>


Connection::Connection(DataSocket* socket)
{
	m_socket = socket;
#ifdef _WIN_32
	m_mutex = CreateMutex(NULL, false, NULL);
#endif
	m_state = CONNECTED;
	m_send_buffer = "";
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

int Connection::StageSend(std::string data)
{
	m_send_buffer.append(data);
	return 0;
}

void Connection::Send(std::string data)
{
	if (m_send_buffer == "")
		return;
	m_send_buffer.append(data);
	std::vector<char> output(m_send_buffer.begin(), m_send_buffer.end());
	output.push_back('\0');
	int sent = m_socket->Send(&output[0]);
	if (sent == -1)
	{
		SetState(DISCONNECTED);
		m_running = false;
	}
	m_send_buffer = "";
}

int Connection::Receive(char* data)
{
    return m_socket->Receive(data);
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
	Thread::Lock(m_mutex);
	m_state = state;
	Thread::Unlock(m_mutex);
}

Connection::State Connection::GetState()
{
	Thread::Lock(m_mutex);
	return m_state;
	Thread::Lock(m_mutex);
}

SOCKET Connection::GetSocket()
{
	return m_socket->GetSocket();
}

void Connection::AddReceivedData(std::string data)
{
	Thread::Lock(m_mutex);
	m_receive_list.push_back(data);
	Thread::Unlock(m_mutex);
}

std::string Connection::GetNextData()
{
	Thread::Lock(m_mutex);
	std::string output = m_receive_list.front();
	m_receive_list.pop_front();
	Thread::Unlock(m_mutex);
	return output;
}

bool Connection::HasMoreData()
{
	Thread::Lock(m_mutex);
	bool result = m_receive_list.size() > 0;
	Thread::Unlock(m_mutex);
	return result;
}

void Connection::SetUsername(std::string username)
{
	m_account.username = username;
}

void Connection::SetPassword(std::string password)
{
	m_account.password = password;
}

void Connection::SetAccount(Account account)
{
	m_account = account;
}

THREAD Connection::ConnectionThread(LPVOID lpParam)
{
	Logger Log = Logger(Logger::L_DEBUG);
	Connection* connection = (Connection*)lpParam;
	char output[1000];
	while (connection->IsRunning())
	{
		memset(output, 0, sizeof(output));
		int result = connection->Receive(output);
		if (result == 0)
		{
			Log.Info("Player Disconnected");
			connection->SetState(DISCONNECTED);
			connection->Stop();
		}
		else if (std::string(output) != "\r\n" && std::string(output) != "")
			connection->AddReceivedData(std::string(output));
	}
	connection->Close();
	return 0;
}

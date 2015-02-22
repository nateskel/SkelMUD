#ifndef CONNECTION_H
#define CONNECTION_H

#ifdef _WIN32
    //#include <windows.h>
    #define THREAD DWORD WINAPI
#else
    #define LPVOID void*
#endif

#include "Network.h"
#include "Thread.h"
#include <string>
#include <list>

class Connection
{
public:
	Connection(DataSocket* socket);
	Connection();
	~Connection();
	struct Account
	{
		std::string username;
		std::string password;
		int id;
	};
	enum State { CONNECTED, DISCONNECTED, USERNAME, PASSWORD, NEWUSER, LOGGEDIN, OOC };
	void Run();
	bool IsRunning();
	void Stop();
	void Close();
	int StageSend(std::string data);
	void Send(std::string data);
	int Receive(char* data);
	HANDLE GetMutex();
	SOCKET GetSocket();
	void AddReceivedData(std::string data);
	std::string GetNextData();
	static THREAD ConnectionThread(LPVOID lpParam);
	bool HasMoreData();
	void SetState(State state);
	State GetState();
	void SetUsername(std::string username);
	void SetPassword(std::string password);
	void SetAccount(Account account);
protected:
private:
	DataSocket* m_socket;
	bool m_running;
	State m_state;
	HANDLE m_mutex;
	std::string m_send_buffer;
	std::list<std::string> m_receive_list;
	Account m_account;
};
#endif // CONNECTION_H

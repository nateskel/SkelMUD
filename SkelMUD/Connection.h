#ifndef CONNECTION_H
#define CONNECTION_H

#ifdef _WIN32
    //#include <windows.h>
    //#define THREAD DWORD WINAPI
#else
    //#define LPVOID void*
#endif

#include "Network.h"
#include "Thread.h"
#include "Logger.h"
#include <string>
#include <list>

class Connection
{
public:
	Logger Log = Logger(Logger::L_DEBUG);
	Connection(DataSocket* socket);
	Connection();
	~Connection();
	enum AccountLevel
	{
		Wizard, GM, Standard, Trial
	};
	struct Account
	{
		std::string username;
		std::string password;
		long id;
		AccountLevel level;
	};
	enum State { CONNECTED, DISCONNECTED, USERNAME, PASSWORD, NEWUSER_CONFIRM, 
		NEWUSER, NEWPASSWORD, CHARACTER_SELECTION,
		LOGGEDIN, OOC };

	bool IsRunning();
	bool HasMoreData();

	void Run();
	void Stop();
	void Close();
	void SetState(State state);
	State GetState();
	void SetUsername(std::string username);
	void SetPassword(std::string password);
	Connection::Account GetAccount();
	void SetAccount(Account account);
	int StageSend(std::string data);
	void Send(std::string data);
	int Receive(char* data);
	HANDLE GetMutex();
	SOCKET GetSocket();
	void AddReceivedData(std::string data);
	std::string GetNextData();
	static THREAD ConnectionThread(LPVOID lpParam);
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

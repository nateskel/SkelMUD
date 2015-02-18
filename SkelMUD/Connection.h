#ifndef CONNECTION_H
#define CONNECTION_H

#ifdef _WIN32
    //#include <windows.h>
    #define THREAD DWORD WINAPI
#else
    #include <pthread.h>
    #define THREAD void*
    #define LPVOID void*
#endif

#include "Network.h"
#include "Callbacks.h"
#include <string>
#include <list>

class Connection
{
public:
	Connection(DataSocket* socket, CallBacks* callback);
	Connection();
	~Connection();
	void Run();
	bool IsRunning();
	void Stop();
	void Close();
	void Send(char* data);
	void Receive(char* data);
	HANDLE GetMutex();
	SOCKET GetSocket();
	void AddReceivedData(std::string data);
	std::string Connection::GetNextData();
	static THREAD ConnectionThread(LPVOID lpParam);
	bool HasMoreData();
	enum State { CONNECTED, USERNAME, PASSWORD, NEWUSER, LOGGEDIN , OOC};
	void SetState(State state);
	State GetState();
protected:
private:
	DataSocket* m_socket;
	bool m_running;
	State m_state;
	CallBacks* m_callback;
	HANDLE m_mutex;
	std::list<std::string> m_receive_list;
	std::list<std::string> m_send_list;
};
#endif // CONNECTION_H

#ifndef THREAD_H
#define THREAD_H

#ifdef _WIN32
#include <thread>
#include <Windows.h>
#define THREAD DWORD WINAPI
#define T_HANDLE HANDLE
#define THREAD_FUNCTION LPTHREAD_START_ROUTINE
#else

#include <pthread.h>

#define THREAD void*
#define HANDLE pthread_mutex_t
#define T_HANDLE int
#define LPVOID void*
#define THREAD_FUNCTION void*
#endif

class Thread {
public:
#ifdef _WIN32
	static T_HANDLE MakeThread(THREAD_FUNCTION thread_function, LPVOID object);
	#else

    static T_HANDLE MakeThread(THREAD_FUNCTION (* thread_function)(void*), LPVOID object);

#endif

    static int Lock(HANDLE mutex);

    static int Unlock(HANDLE mutex);

private:

};

#endif // THREAD_H

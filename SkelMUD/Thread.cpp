#include "Thread.h"


int Thread::Lock(HANDLE mutex)
{
#ifdef _WIN32
	WaitForSingleObject(mutex, INFINITE);
#else
    pthread_mutex_lock(&mutex);
#endif
    return 0;
}

int Thread::Unlock(HANDLE mutex)
{
#ifdef _WIN32
	ReleaseMutex(mutex);
#else
    pthread_mutex_unlock(&mutex);
#endif
   return 0;
}

T_HANDLE Thread::MakeThread(THREAD_FUNCTION thread_function, LPVOID object)
{
#ifdef _WIN32
	DWORD dwThreadId;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, thread_function, object, 0, &dwThreadId);
#else
	pthread_t ptThreadID;
	int hThread;
	hThread = pthread_create(&ptThreadID, NULL, thread_function, object);
#endif
	return hThread;
}
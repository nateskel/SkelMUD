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
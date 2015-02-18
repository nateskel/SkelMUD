#ifndef THREAD_H
#define THREAD_H

#ifndef _WIN32
#include <pthread.h>
#define THREAD void*
#define HANDLE pthread_mutex_t
#endif

class Thread
{
public:
	Thread();
	~Thread();
	static int Lock(HANDLE mutex);
	static int Unlock(HANDLE mutex);

private:


};

#endif

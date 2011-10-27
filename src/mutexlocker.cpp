#include "bu/mutexlocker.h"
#include "bu/mutex.h"

Bu::MutexLocker::MutexLocker( Bu::Mutex &mu ) :
	mu( mu )
{
	mu.lock();
}

Bu::MutexLocker::~MutexLocker()
{
	mu.unlock();
}

void Bu::MutexLocker::unlock()
{
	mu.unlock();
}

void Bu::MutexLocker::relock()
{
	mu.lock();
}


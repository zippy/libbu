#include "bu/itolocker.h"
#include "bu/itomutex.h"

Bu::ItoLocker::ItoLocker( Bu::ItoMutex &mu ) :
	mu( mu )
{
	mu.lock();
}

Bu::ItoLocker::~ItoLocker()
{
	mu.unlock();
}

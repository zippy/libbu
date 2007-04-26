#include "itomutex.h"

Bu::ItoMutex::ItoMutex()
{
	pthread_mutex_init( &mutex, NULL );
}

Bu::ItoMutex::~ItoMutex()
{
	pthread_mutex_destroy( &mutex );
}

int Bu::ItoMutex::lock()
{
	return pthread_mutex_lock( &mutex );
}

int Bu::ItoMutex::unlock()
{
	return pthread_mutex_unlock( &mutex );
}

int Bu::ItoMutex::trylock()
{
	return pthread_mutex_trylock( &mutex );
}


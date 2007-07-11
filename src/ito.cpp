#include "ito.h"

Bu::Ito::Ito()
{
}

Bu::Ito::~Ito()
{
}

bool Bu::Ito::start()
{
	nHandle = pthread_create( &ptHandle, NULL, threadRunner, this );

	return true;
}

bool Bu::Ito::stop()
{
	pthread_exit( &ptHandle );

	return true;
}

void *Bu::Ito::threadRunner( void *pThread )
{
	return ((Ito *)pThread)->run();
}

bool Bu::Ito::join()
{
	pthread_join( ptHandle, NULL );
	return true;
}

void Bu::Ito::yield()
{
	pthread_yield();
}

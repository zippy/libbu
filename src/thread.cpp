/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/thread.h"

#include "bu/config.h"

Bu::Thread::Thread()
{
}

Bu::Thread::~Thread()
{
}

bool Bu::Thread::start()
{
	nHandle = pthread_create( &ptHandle, NULL, threadRunner, this );

	return true;
}

bool Bu::Thread::stop()
{
	pthread_cancel( ptHandle );

	return true;
}

void *Bu::Thread::threadRunner( void *pThread )
{
	((Thread *)pThread)->run();
	pthread_exit( NULL );
	return NULL;
}

bool Bu::Thread::join()
{
	pthread_join( ptHandle, NULL );
	return true;
}

void Bu::Thread::yield()
{
#ifndef WIN32
	pthread_yield();
#else
	#warning Bu::Thread::yield IS A STUB for WIN32!!!!	
#endif
}


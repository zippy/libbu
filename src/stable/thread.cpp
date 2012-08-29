/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/thread.h"

namespace Bu { subExceptionDef( ThreadException ); }

Bu::ThreadId::ThreadId( pthread_t tId ) :
	tId( tId )
{
}

Bu::ThreadId::ThreadId()
{
}

bool Bu::ThreadId::operator==( const Bu::ThreadId &rhs )
{
	return pthread_equal( tId, rhs.tId );
}

bool Bu::ThreadId::operator!=( const ThreadId &rhs )
{
	return !pthread_equal( tId, rhs.tId );
}

Bu::Thread::Thread()
{
}

Bu::Thread::~Thread()
{
}

Bu::ThreadId Bu::Thread::currentThread()
{
	return ThreadId( pthread_self() );
}

bool Bu::Thread::start()
{
	if( pthread_create( &ptHandle, NULL, threadRunner, this ) )
	{
		throw Bu::ThreadException("Could not start thread.");
	}

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
	sched_yield();
#endif
}


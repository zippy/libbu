/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/ito.h"
#include "bu/osx_compatibility.h"

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
#ifndef WIN32
	pthread_yield();
#else
	#warning Bu::Ito::yield IS A STUB for WIN32!!!!	
#endif
}


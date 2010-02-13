/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/itomutex.h"

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


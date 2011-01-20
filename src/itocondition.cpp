/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <sys/time.h>

#include "bu/itocondition.h"

Bu::ItoCondition::ItoCondition()
{
	pthread_cond_init( &cond, NULL );
}

Bu::ItoCondition::~ItoCondition()
{
	pthread_cond_destroy( &cond );
}

int Bu::ItoCondition::wait()
{
	return pthread_cond_wait( &cond, &mutex );
}

int Bu::ItoCondition::wait( int nSec, int nUSec )
{
	struct timeval now;
	struct timespec timeout;
	struct timezone tz;
	
	gettimeofday( &now, &tz );
	timeout.tv_sec = now.tv_sec + nSec + ((now.tv_usec + nUSec)/1000000);
	timeout.tv_nsec = ((now.tv_usec + nUSec)%1000000)*1000;

	return pthread_cond_timedwait( &cond, &mutex, &timeout );
}

int Bu::ItoCondition::signal()
{
	return pthread_cond_signal( &cond );
}

int Bu::ItoCondition::broadcast()
{
	return pthread_cond_broadcast( &cond );
}


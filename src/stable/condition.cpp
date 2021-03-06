/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <sys/time.h>

#include "bu/condition.h"

Bu::Condition::Condition()
{
    pthread_cond_init( &cond, NULL );
}

Bu::Condition::~Condition()
{
    pthread_cond_destroy( &cond );
}

int Bu::Condition::wait()
{
    return pthread_cond_wait( &cond, &mutex );
}

int Bu::Condition::wait( int nSec, int nUSec )
{
    struct timeval now;
    struct timespec timeout;
    struct timezone tz;
    
    gettimeofday( &now, &tz );
    timeout.tv_sec = now.tv_sec + nSec + ((now.tv_usec + nUSec)/1000000);
    timeout.tv_nsec = ((now.tv_usec + nUSec)%1000000)*1000;

    return pthread_cond_timedwait( &cond, &mutex, &timeout );
}

int Bu::Condition::signal()
{
    return pthread_cond_signal( &cond );
}

int Bu::Condition::broadcast()
{
    return pthread_cond_broadcast( &cond );
}


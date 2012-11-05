/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/mutex.h"

Bu::Mutex::Mutex()
{
    pthread_mutex_init( &mutex, NULL );
}

Bu::Mutex::~Mutex()
{
    pthread_mutex_destroy( &mutex );
}

int Bu::Mutex::lock()
{
    return pthread_mutex_lock( &mutex );
}

int Bu::Mutex::unlock()
{
    return pthread_mutex_unlock( &mutex );
}

int Bu::Mutex::trylock()
{
    return pthread_mutex_trylock( &mutex );
}


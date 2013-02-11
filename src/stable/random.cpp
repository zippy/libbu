/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#include "bu/random.h"

#include "bu/randombasic.h"

Bu::Random::Random() :
    pGen( NULL )
{
    pGen = new RandomBasic();
}

Bu::Random::~Random()
{
    delete pGen;
    pGen = NULL;
}

void Bu::Random::seed( int32_t iSeed )
{
    getInstance().pGen->seed( iSeed );
}

int32_t Bu::Random::rand()
{
    return getInstance().pGen->rand();
}

int32_t Bu::Random::rand( int32_t iMax )
{
    return getInstance().pGen->rand( iMax );
}

int32_t Bu::Random::rand( int32_t iMin, int32_t iMax )
{
    return getInstance().pGen->rand( iMin, iMax );
}

double Bu::Random::randNorm()
{
    return getInstance().pGen->randNorm();
}


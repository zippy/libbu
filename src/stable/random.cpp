/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
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

int32_t Bu::Random::rand()
{
	return getInstance().pGen->rand();
}

void Bu::Random::seed( int32_t iSeed )
{
	getInstance().pGen->seed( iSeed );
}


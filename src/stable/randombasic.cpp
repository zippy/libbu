/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/randombasic.h"

Bu::RandomBasic::RandomBasic() :
	a( 6364136223846793005ll ),
	c( 1442695040888963407ll ),
	x( 0 )
{
}

Bu::RandomBasic::~RandomBasic()
{
}

void Bu::RandomBasic::seed( int32_t iSeed )
{
	c = iSeed;
}

int32_t Bu::RandomBasic::rand()
{
	x = (a*x + c);
	return (int32_t)x;
}


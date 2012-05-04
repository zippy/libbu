/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/randomcmwc.h"

#define PHI 0x9e3779b9

RandomCmwc::RandomCmwc() :
	q( NULL ),
	c( 362436 )
{
	q = new uint32_t[4096];
}

RandomCmwc::~RandomCmwc()
{
	delete[] q;
}

void RandomCmwc::seed( int32_t iSeed )
{
	int i;

	Q[0] = iSeed;
	Q[1] = iSeed + PHI;
	Q[2] = iSeed + PHI + PHI;

	for (i = 3; i < 4096; i++)
		Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;	
}

int32_t RandomCmwc::rand()
{
	uint64_t t, a = 18782LL;
	static uint32_t i = 4095;
	uint32_t x, r = 0xfffffffe;
	i = (i + 1) & 4095;
	t = a * Q[i] + c;
	c = (t >> 32);
	x = t + c;
	if( x < c )
	{
		x++;
		c++;
	}
	return (Q[i] = r - x);
}


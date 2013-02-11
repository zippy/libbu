/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/randomcmwc.h"

#include "bu/randombasic.h"

#define PHI 0x9e3779b9

Bu::RandomCmwc::RandomCmwc( int32_t iSeed ) :
    q( 0 ),
    c( 362436 )
{
    q = new uint32_t[4096];
    seed( iSeed );
}

Bu::RandomCmwc::~RandomCmwc()
{
    delete[] q;
}

void Bu::RandomCmwc::seed( int32_t iSeed )
{
    i = 4095;
    c = 362436;

    Bu::RandomBasic rb;
    rb.seed( iSeed );

    for( int j = 0; j < 4096; j++ )
        q[j] = rb.rand();

    c = rb.rand();

    return;
    q[0] = (uint32_t)rb.rand();
    q[1] = (uint32_t)rb.rand() + PHI;
    q[2] = (uint32_t)rb.rand() + PHI + PHI;

    for (int j = 3; j < 4096; j++)
        q[j] = q[j - 3] ^ q[j - 2] ^ PHI ^ j;
}

int32_t Bu::RandomCmwc::rand()
{
    uint64_t t, a = 18782LL;
    uint32_t x, r = 0xfffffffe;
    i = (i + 1) & 4095;
    t = a * q[i] + c;
    c = (t >> 32);
    x = t + c;
    if( x < c )
    {
        x++;
        c++;
    }
    return (q[i] = r - x);
}


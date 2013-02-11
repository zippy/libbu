/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_BASIC_H
#define BU_RANDOM_BASIC_H

#include "bu/randombase.h"

namespace Bu
{
    /**
     * Basic Linear Congruential Generator.  This is one of the simplest and
     * most common pseudo-random number generators out there.  It's simple,
     * fast, and does a decent job if you don't have any stastistical
     * requirements other than "random looking."  This is the algorithm included
     * as the default random number generator in most standard libraries,
     * although this one uses larger numbers internally, has a longer period,
     * and more even distrubition of randomness accross it's bits than most
     * implementations.
     *
     * The base values were published by Donald Knuth.
     */
    class RandomBasic : public RandomBase
    {
    public:
        RandomBasic( int32_t iSeed=0 );
        virtual ~RandomBasic();

        virtual void seed( int32_t iSeed );

        virtual int32_t rand();

    private:
        int64_t x;
    };
};

#endif

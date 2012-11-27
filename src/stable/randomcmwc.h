/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_CMWC_H
#define BU_RANDOM_CMWC_H

#include "bu/randombase.h"

namespace Bu
{
    /**
     * Complimentary Multiply With Carry random number generator.  This is a
     * simpler and faster generator than Mersenne Twister, but uses about as
     * much memory.  Many people think that it may be at least as good as
     * Mersenne Twister, or better.
     */
    class RandomCmwc : public RandomBase
    {
    public:
        RandomCmwc( int32_t iSeed=0 );
        virtual ~RandomCmwc();

        virtual void seed( int32_t iSeed );

        virtual int32_t rand();

    private:
        uint32_t *q, c, i;
    };
};

#endif

/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_SYSTEM_H
#define BU_RANDOM_SYSTEM_H

#include "bu/randombase.h"

namespace Bu
{
    class File;
    /**
     * Access to operating system provided random number generation.  On linux
     * this will provide access to /dev/random or /dev/urandom depending on
     * weather Fast or Good is selected.  On Linux systems Good random number
     * generation can block while organic sources are use to gather entropy, so
     * Fast is the default, it will always provide a result.
     *
     * On Windows this uses the Windows system cryptographic level random number
     * generation services.  Or at least, it will when I get to it.  Sorry.
     */
    class RandomSystem : public RandomBase
    {
    public:
        enum Type
        {
            Fast,
            Good
        };

        RandomSystem( Type eType=Fast );
        virtual ~RandomSystem();

        virtual void seed( int32_t iSeed );

        virtual int32_t rand();

    private:
        Type eType;
        File *pSrc;
    };
};

#endif

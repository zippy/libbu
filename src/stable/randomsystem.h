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

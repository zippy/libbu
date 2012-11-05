/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_BASIC_H
#define BU_RANDOM_BASIC_H

#include "bu/randombase.h"

namespace Bu
{
    class RandomBasic : public RandomBase
    {
    public:
        RandomBasic();
        virtual ~RandomBasic();

        virtual void seed( int32_t iSeed );

        virtual int32_t rand();

    private:
        int64_t a, c, x;
    };
};

#endif

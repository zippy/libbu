/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_BASE_H
#define BU_RANDOM_BASE_H

#include <stdint.h>

namespace Bu
{
    class RandomBase
    {
    public:
        RandomBase();
        virtual ~RandomBase();

        virtual void seed( int32_t iSeed )=0;
        virtual int32_t rand()=0;
        virtual inline int32_t rand( int32_t iMax ) {
            return rand( 0, iMax );
        }
        virtual inline int32_t rand( int32_t iMin, int32_t iMax ) {
            return iMin+(randNorm()*(iMax-iMin));
        }
        virtual inline double randNorm() {
            return (((uint32_t)rand())&0xfffffffeul)*0x1.00000001p-32;
//          return (((uint32_t)rand())&0xfffffffeul)/(double)(0xfffffffful);
        }
    };
};

#endif

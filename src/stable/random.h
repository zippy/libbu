/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */
#ifndef BU_RANDOM_H
#define BU_RANDOM_H

#include "bu/singleton.h"
#include <stdint.h>

namespace Bu
{
    class RandomBase;
    class Random : public Bu::Singleton<Bu::Random>
    {
    friend class Bu::Singleton<Bu::Random>;
    private:
        Random();
        virtual ~Random();

    public:
        template<typename cl>
        static void setGenerator()
        {
            delete getInstance().pGen;
            getInstance().pGen = new cl();
        }
        
        template<typename cl, typename t1>
        static void setGenerator( t1 p1 )
        {
            delete getInstance().pGen;
            getInstance().pGen = new cl( p1 );
        }
        
        template<typename cl, typename t1, typename t2>
        static void setGenerator( t1 p1, t2 p2 )
        {
            delete getInstance().pGen;
            getInstance().pGen = new cl( p1, p2 );
        }
        
        RandomBase &getGenerator() { return *pGen; }

        static void seed( int32_t iSeed );
        static int32_t rand();
        static int32_t rand( int32_t iMax );
        static int32_t rand( int32_t iMin, int32_t iMax );
        static double randNorm();

    private:
        void checkInit();

    private:
        Bu::RandomBase *pGen;
    };
};

#endif


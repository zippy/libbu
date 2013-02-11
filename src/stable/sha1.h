/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef SHA1_H
#define SHA1_H

#include <stdint.h>
#include "bu/cryptohash.h"

namespace Bu
{
    /**
     * Calculates SHA-1 sums.  This is based strongly on code from Michael D.
     * Leonhard who released his code under the terms of the MIT license,
     * thank you!  Check out his website http://tamale.net he has a lot of
     * cool stuff there.
     */
    class Sha1 : public CryptoHash
    {
    public:
        Sha1();
        virtual ~Sha1();

        virtual void reset();
        virtual void setSalt( const Bu::String &sSalt );
        virtual void addData( const void *sData, int iSize );
        using CryptoHash::addData;
        virtual String getResult();
        virtual void writeResult( Stream &sOut );

        void update( const char* data, int num );
        unsigned char* getDigest();

        // utility methods

    private:
        static uint32_t lrot( uint32_t x, int bits );
        static void toBigEndian( uint32_t in, unsigned char* out );
        void process();
    
    private:
        uint32_t uH0, uH1, uH2, uH3, uH4;
        unsigned char uBytes[64];
        int iUnprocessedBytes;
        uint32_t uTotalBytes;
    };
};

#endif

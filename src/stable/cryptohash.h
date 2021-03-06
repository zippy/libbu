/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CRYPTO_HASH_H
#define BU_CRYPTO_HASH_H

#include "bu/string.h"

namespace Bu
{
    class Stream;

    class CryptoHash
    {
    public:
        CryptoHash();
        virtual ~CryptoHash();

        virtual void reset() = 0;
        virtual void setSalt( const Bu::String &sSalt ) = 0;
        virtual void addData( const void *sData, int iSize ) = 0;
        virtual void addData( const Bu::String &sData );
        virtual String getResult() = 0;
        virtual void writeResult( Stream &sOut ) = 0;
        virtual Bu::String getHexResult();
    };
};

#endif

/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_CIPHER_H
#define BU_CIPHER_H

#include "bu/filter.h"
#include "bu/util.h"

namespace Bu
{
    /**
     * Represents a nice base class for a stream filter block encryption scheme.
     * This class handles efficient caching during reading and writing,
     * encrypting and decrypting for block ciphers.  For each individual cipher
     * you only have to worry about the block encryption and decryption.  Cipher
     * modes are handled with seperate classes, see Bu::CipherModeCbc,
     * Bu::CipherModeCfb, Bu::CipherModeEcb, and Bu::CipherModeOfb.
     *
     *
     */
    template<int iBlockSize>
    class Cipher : public Bu::Filter
    {
    public:
        Cipher( Bu::Stream &rNext ) :
            Bu::Filter( rNext ),
            iReadBufFill( 0 ),
            iReadBufPos( 0 ),
            iWriteBufFill( 0 )
        {
        }

        virtual ~Cipher()
        {
        }

        virtual void start()
        {
        }

        virtual Bu::size stop()
        {
            flush();
            return 0;
        }

        virtual Bu::size read( void *pBuf, Bu::size iBytes )
        {
            Bu::size iRead = 0;
            while( iRead < iBytes )
            {
                if( iReadBufFill < iBlockSize )
                {
                    int iR = rNext.read(
                        aReadBuf+iReadBufFill,
                        iBlockSize-iReadBufFill
                        );
                    if( iR == 0 )
                        return iRead;

                    iReadBufFill += iR;

                    if( iReadBufFill == iBlockSize )
                        decipher( aReadBuf );
                }

                if( iReadBufFill == iBlockSize )
                {
                    int iCpy = Bu::buMin( (int)(iBytes-iRead), iBlockSize-iReadBufPos );
                    memcpy( ((char *)pBuf)+iRead, aReadBuf+iReadBufPos, iCpy );
                    iRead += iCpy;
                    iReadBufPos += iCpy;
                    if( iReadBufPos == iBlockSize )
                    {
                        iReadBufPos = iReadBufFill = 0;
                    }
                }
            }

            return iRead;
        }

        virtual Bu::size write( const void *pBuf, Bu::size iBytes )
        {
            Bu::size iPos = 0;
            
            while( iPos < iBytes )
            {
                int iLeft = Bu::buMin((int)(iBytes-iPos),iBlockSize-iWriteBufFill);
                memcpy( aWriteBuf+iWriteBufFill, (char *)pBuf+iPos, iLeft );
                iPos += iLeft;
                iWriteBufFill += iLeft;
                if( iWriteBufFill == iBlockSize )
                {
                    encipher( aWriteBuf );
                    rNext.write( aWriteBuf, iBlockSize );
                    iWriteBufFill = 0;
                }
            }

            return iPos;
        }

        virtual void flush()
        {
            if( iWriteBufFill > 0 && iWriteBufFill < iBlockSize )
            {
                memset( aWriteBuf+iWriteBufFill, 0, iBlockSize-iWriteBufFill );
                encipher( aWriteBuf );
                rNext.write( aWriteBuf, iBlockSize );
                iWriteBufFill = 0;
            }
            rNext.flush();
        }

        using Bu::Stream::read;
        using Bu::Stream::write;

    protected:
        virtual void encipher( void *pData )=0;
        virtual void decipher( void *pData )=0;

    private:
        char aReadBuf[iBlockSize];
        char aWriteBuf[iBlockSize];
        int iReadBufFill;
        int iReadBufPos;
        int iWriteBufFill;
    };
};

#endif

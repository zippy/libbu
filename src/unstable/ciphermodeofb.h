#ifndef BU_MODE_OFB_H
#define BU_MODE_OFB_H

#include "bu/filter.h"
#include "bu/string.h"

namespace Bu
{
    /**
     * Output Feedback Mode.  This cipher mode is one of the most resiliant.
     * Instead of encrypting your data directly it encrypts a "key stream" using
     * the initialization vector, and then XORs those blocks with your stream
     * blocks.  This means that an error in your stream will still produce an
     * error in the output, but it will not propegate.  Also, with most
     * encryption schemes error correction codes on the source data will still
     * work on the encrypted data or decrypted output.
     */
    template<int iBlockSize, typename CipherType>
    class CipherModeOfb : public CipherType
    {
    public:
        CipherModeOfb(class Stream &rNext ) :
            CipherType( rNext ),
            bStart( true )
        {
            memset( aVector, 0, iBlockSize );
        }

        virtual ~CipherModeOfb()
        {
        }

        void setIv( const Bu::String &sIv )
        {
            memcpy( aVector, sIv.getStr(), iBlockSize );
        }

    protected:
        void decipher( void *pBuf )
        {
            CipherType::encipher( aVector );
            uint8_t aTmp[iBlockSize];
            memcpy( aTmp, aVector, iBlockSize );
            for( int j = 0; j < iBlockSize; j++ )
                ((uint8_t *)pBuf)[j] ^= aVector[j];
            memcpy( aVector, aTmp, iBlockSize );
        }

        void encipher( void *pBuf )
        {
            CipherType::encipher( aVector );
            uint8_t aTmp[iBlockSize];
            memcpy( aTmp, aVector, iBlockSize );
            for( int j = 0; j < iBlockSize; j++ )
                ((uint8_t *)pBuf)[j] ^= aVector[j];
            memcpy( aVector, aTmp, iBlockSize );
        }

    private:
        bool bStart;
        uint8_t aVector[iBlockSize];
    };
};

#endif

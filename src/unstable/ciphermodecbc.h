#ifndef BU_MODE_CBC_H
#define BU_MODE_CBC_H

#include "bu/filter.h"
#include "bu/string.h"

namespace Bu
{
    /**
     * Cipher-block chaining mode.  The Initialization Vector (IV) is fed into
     * the first block, then each subsequent block is fed into the next making
     * each block dependant on all previous blocks.
     */
    template<int iBlockSize, typename CipherType>
    class CipherModeCbc : public CipherType
    {
    public:
        CipherModeCbc(class Stream &rNext ) :
            CipherType( rNext ),
            bStart( true )
        {
            memset( aVector, 0, iBlockSize );
        }

        virtual ~CipherModeCbc()
        {
        }

        void setIv( const Bu::String &sIv )
        {
            memcpy( aVector, sIv.getStr(), iBlockSize );
        }

    protected:
        void decipher( void *pBuf )
        {
            uint8_t aTmp[iBlockSize];
            memcpy( aTmp, pBuf, iBlockSize );
            CipherType::decipher( pBuf );
            for( int j = 0; j < iBlockSize; j++ )
                ((uint8_t *)pBuf)[j] ^= aVector[j];
            memcpy( aVector, aTmp, iBlockSize );
        }

        void encipher( void *pBuf )
        {
            for( int j = 0; j < iBlockSize; j++ )
                ((uint8_t *)pBuf)[j] ^= aVector[j];
            CipherType::encipher( pBuf );
            memcpy( aVector, pBuf, iBlockSize );
        }

    private:
        bool bStart;
        uint8_t aVector[iBlockSize];
    };
};

#endif

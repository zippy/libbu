#ifndef BU_MODE_CFB_H
#define BU_MODE_CFB_H

#include "bu/filter.h"
#include "bu/string.h"

namespace Bu
{
    /**
     * Cipher Feedback mode.  This is very similar to the Cipher-block chaining
     * mode, with a slight tweak (Bu::CipherModeCbc).  Each block is still
     * dependant on all previous blocks.  Any corruption and the entire stream
     * will be corrupt.
     */
    template<int iBlockSize, typename CipherType>
    class CipherModeCfb : public CipherType
    {
    public:
        CipherModeCfb(class Stream &rNext ) :
            CipherType( rNext ),
            bStart( true )
        {
            memset( aVector, 0, iBlockSize );
        }

        virtual ~CipherModeCfb()
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
            CipherType::encipher( aVector );
            for( int j = 0; j < iBlockSize; j++ )
                ((uint8_t *)pBuf)[j] ^= aVector[j];
            memcpy( aVector, aTmp, iBlockSize );
        }

        void encipher( void *pBuf )
        {
            CipherType::encipher( aVector );
            for( int j = 0; j < iBlockSize; j++ )
                aVector[j] = ((uint8_t *)pBuf)[j] ^= aVector[j];
        }

    private:
        bool bStart;
        uint8_t aVector[iBlockSize];
    };
};

#endif

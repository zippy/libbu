#ifndef BU_MODE_ECB_H
#define BU_MODE_ECB_H

namespace Bu
{
    template<int iBlockSize, typename CipherType>
    class CipherModeEcb : public CipherType
    {
    public:
        CipherModeEcb( class Stream &rNext ) :
            CipherType( rNext )
        {
        }

        virtual ~CipherModeEcb()
        {
        }
    
    protected:
        virtual void decipher( void *pBuf )
        {
            CipherType::decipher( pBuf );
        }

        virtual void encipher( void *pBuf )
        {
            CipherType::encipher( pBuf );
        }
    };
};

#endif

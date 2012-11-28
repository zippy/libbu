#ifndef BU_MODE_ECB_H
#define BU_MODE_ECB_H

namespace Bu
{
    /**
     * Electronic Code Book mode.  This cipher mode is the simplest, it's
     * effectively a pass-through mode.  It's the same as using the encryption
     * scheme without a mode, but at least you absolutely know that you've got
     * the correct mode.  I recommend using this instead of the raw mode if for
     * no other reason than it makes your code more self-documenting, and with
     * optomization shouldn't add any extra calls to your code.
     */
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

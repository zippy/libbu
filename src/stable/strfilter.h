#ifndef STR_FILTER_H
#define STR_FILTER_H

#include "bu/string.h"
#include "bu/membuf.h"

namespace Bu
{
    //
    // Encoders
    //
    template<typename tFilter>
    Bu::String encodeStr( const Bu::String &sIn )
    {
        Bu::MemBuf mb;
        {
            tFilter fEnc( mb );
            fEnc.write( sIn.getStr(), sIn.getSize() );
        }
        return mb.getString();
    }

    template<typename tFilter, typename p1t>
    Bu::String encodeStr( const Bu::String &sIn, p1t p1 )
    {
        Bu::MemBuf mb;
        {
            tFilter fEnc( mb, p1 );
            fEnc.write( sIn.getStr(), sIn.getSize() );
        }
        return mb.getString();
    }

    template<typename tFilter, typename p1t, typename p2t>
    Bu::String encodeStr( const Bu::String &sIn, p1t p1, p2t p2 )
    {
        Bu::MemBuf mb;
        {
            tFilter fEnc( mb, p1, p2 );
            fEnc.write( sIn.getStr(), sIn.getSize() );
        }
        return mb.getString();
    }

    template<typename tFilter, typename p1t, typename p2t, typename p3t>
    Bu::String encodeStr( const Bu::String &sIn, p1t p1, p2t p2, p3t p3 )
    {
        Bu::MemBuf mb;
        {
            tFilter fEnc( mb, p1, p2 );
            fEnc.write( sIn.getStr(), sIn.getSize() );
        }
        return mb.getString();
    }

    //
    // Decoders
    //
    template<typename tFilter>
    Bu::String decodeStr( const Bu::String &sIn )
    {
        Bu::MemBuf mb( sIn );
        tFilter fDec( mb );
        char buf[1024];
        String sRet;
        for(;;)
        {
            int iRead = fDec.read( buf, 1024 );
            if( iRead == 0 )
                return sRet;
            sRet.append( buf, iRead );
        }
    }
    
    template<typename tFilter, typename p1t>
    Bu::String decodeStr( const Bu::String &sIn, p1t p1 )
    {
        Bu::MemBuf mb( sIn );
        tFilter fDec( mb, p1 );
        char buf[1024];
        String sRet;
        for(;;)
        {
            int iRead = fDec.read( buf, 1024 );
            if( iRead == 0 )
                return sRet;
            sRet.append( buf, iRead );
        }
    }
    
    template<typename tFilter, typename p1t, typename p2t>
    Bu::String decodeStr( const Bu::String &sIn, p1t p1, p2t p2 )
    {
        Bu::MemBuf mb( sIn );
        tFilter fDec( mb, p1, p2 );
        char buf[1024];
        String sRet;
        for(;;)
        {
            int iRead = fDec.read( buf, 1024 );
            if( iRead == 0 )
                return sRet;
            sRet.append( buf, iRead );
        }
    }
    
    template<typename tFilter, typename p1t, typename p2t, typename p3t>
    Bu::String decodeStr( const Bu::String &sIn, p1t p1, p2t p2, p3t p3 )
    {
        Bu::MemBuf mb( sIn );
        tFilter fDec( mb, p1, p2, p3 );
        char buf[1024];
        String sRet;
        for(;;)
        {
            int iRead = fDec.read( buf, 1024 );
            if( iRead == 0 )
                return sRet;
            sRet.append( buf, iRead );
        }
    }
};

#endif

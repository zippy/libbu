/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/stream.h"

Bu::Stream::Stream()
{
}

Bu::Stream::~Stream()
{
}

Bu::String Bu::Stream::readLine()
{
    Bu::String sRet;
    
    for(;;)
    {
        char s;
        if( read( &s, 1 ) == 0 )
            return sRet;
        if( s == '\n' || s == '\r' )
            return sRet;
        sRet.append( s );
    }
}

Bu::String Bu::Stream::readAll()
{
    Bu::String sRet;
    char buf[4096];

    while( !isEos() )
    {
        int iRead = read( buf, 4096 );
        if( iRead == 0 )
            return sRet;
        sRet.append( buf, iRead );
    }

    return sRet;
}

Bu::size Bu::Stream::write( const Bu::String &sBuf )
{
    return write( sBuf.getStr(), sBuf.getSize() );
}


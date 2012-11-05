/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sio.h"
#include "bu/streamstack.h"
#include "bu/stdstream.h"

Bu::StreamStack Bu::sioRaw( new Bu::StdStream() );
Bu::Formatter Bu::sio( Bu::sioRaw );
Bu::StreamStack Bu::serrRaw( new Bu::StdStream( Bu::StdStream::StdError ) );
Bu::Formatter Bu::serr( Bu::serrRaw );

namespace Bu
{
    class PrintEndAction : public Bu::String::FormatProxyEndAction
    {
    public:
        PrintEndAction( Bu::Stream &s, bool bEndLn ) :
            s( s ),
            bEndLn( bEndLn )
        {
        }

        virtual ~PrintEndAction()
        {
        }

        virtual void operator()( const Bu::String &sFinal )
        {
            s.write( sFinal.getStr(), sFinal.getSize() );
            if( bEndLn )
            {
                s.write("\n", 1);
                s.flush();
            }
        }

        Bu::Stream &s;
        bool bEndLn;
    };
}

Bu::String::FormatProxy Bu::print( Bu::Stream &s, const Bu::String &str )
{
    return str.format( new Bu::PrintEndAction( s, false ) );
}

Bu::String::FormatProxy Bu::print( const Bu::String &str )
{
    return print( sioRaw, str );
}

Bu::String::FormatProxy Bu::println( Bu::Stream &s, const Bu::String &str )
{
    return str.format( new Bu::PrintEndAction( s, true ) );
}

Bu::String::FormatProxy Bu::println( const Bu::String &str )
{
    return println( sioRaw, str );
}


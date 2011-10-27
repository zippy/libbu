/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/sio.h"

Bu::StdStream Bu::sioRaw;
Bu::Formatter Bu::sio( Bu::sioRaw );

Bu::size Bu::print( Bu::Stream &s, const Bu::String &str )
{
	return s.write( str.getStr(), str.getSize() );
}

Bu::size Bu::print( const Bu::String &str )
{
	return print( sioRaw, str );
}

Bu::size Bu::println( Bu::Stream &s, const Bu::String &str )
{
	Bu::size sRet = s.write( str.getStr(), str.getSize() );
	sRet += s.write("\n", 1 );
	s.flush();
	return sRet;
}

Bu::size Bu::println( const Bu::String &str )
{
	return println( sioRaw, str );
}


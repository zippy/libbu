/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
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

Bu::FString Bu::Stream::readLine()
{
	Bu::FString sRet;
	
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

size_t Bu::Stream::write( const Bu::FString &sBuf )
{
	return write( sBuf.getStr(), sBuf.getSize() );
}


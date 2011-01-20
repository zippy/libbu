/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/newline.h"

Bu::NewLine::NewLine( Bu::Stream &rNext ) :
	Bu::Filter( rNext ),
	bExChar( false )
{
}

Bu::NewLine::~NewLine()
{
}

void Bu::NewLine::start()
{
}

Bu::size Bu::NewLine::stop()
{
	return 0;
}

Bu::size Bu::NewLine::read( void *pBufV, Bu::size iAmnt )
{
	Bu::size iTotal = 0;
	Bu::size iOffset = 0;
	Bu::size iRead = rNext.read( pBufV, iAmnt );
	char *pBuf = (char *)pBufV;

	for( Bu::size i = 0; i < iRead; i++ )
	{
		if( pBuf[i] == '\r' )
		{
			pBuf[i+iOffset] = '\n';
			if( pBuf[i+1] == '\n' )
			{
				iOffset--;
			}
		}
		else if( pBuf[i] == '\n' )
		{
			pBuf[i+iOffset] = '\n';
			if( pBuf[i+1] == '\r' )
			{
				iOffset--;
			}
		}
		else if( iOffset )
		{
			pBuf[i+iOffset] = pBuf[i];
		}
	}

	iTotal += iRead + iOffset;
	return iTotal;
}

Bu::size Bu::NewLine::write( const void *, Bu::size )
{
	return 0;
}


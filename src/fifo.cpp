/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/fifo.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "win32_compatibility.h"

namespace Bu { subExceptionDef( FifoException ) }

Bu::Fifo::Fifo( const Bu::FString &sName, int iFlags, mode_t mAcc ) :
	iFlags( iFlags ),
	iIn( -1 ),
	iOut( -1 )
{
#ifndef WIN32
	if( iFlags&Create )
	{
		if( mkfifo( sName.getStr(), mAcc ) )
		{
			throw FifoException("Error creating fifo: %s\n", strerror( errno ) );
		}
	}
	if( iFlags&Read )
	{
		iIn = ::open(
			sName.getStr(),
			O_RDONLY|((iFlags&NonBlock)?O_NONBLOCK:0)
			);
	}
	if( iFlags&Write )
	{
		iOut = ::open(
			sName.getStr(),
			O_WRONLY
			);
	}
#else
	#warning Bu::Fifo::Fifo IS A STUB for WIN32!!!!	
#endif
}

Bu::Fifo::~Fifo()
{
	close();
}

void Bu::Fifo::close()
{
	if( iIn > -1 )
	{
		::close( iIn );
		iIn = -1;
	}
	if( iOut > -1 )
	{
		::close( iOut );
		iOut = -1;
	}
}

size_t Bu::Fifo::read( void *pBuf, size_t nBytes )
{
	if( iIn < 0 )
		throw FifoException("Fifo not open for reading.");

	return TEMP_FAILURE_RETRY( ::read( iIn, pBuf, nBytes ) );
}

size_t Bu::Fifo::write( const void *pBuf, size_t nBytes )
{
	if( iOut < 0 )
		throw FifoException("Fifo not open for writing.");

	return TEMP_FAILURE_RETRY( ::write( iOut, pBuf, nBytes ) );
}

long Bu::Fifo::tell()
{
	return -1;
}

void Bu::Fifo::seek( long )
{
}

void Bu::Fifo::setPos( long )
{
}

void Bu::Fifo::setPosEnd( long )
{
}

bool Bu::Fifo::isEos()
{
	return false;
}

bool Bu::Fifo::canRead()
{
	return (iIn>-1);
}

bool Bu::Fifo::canWrite()
{
	return (iOut>-1);
}

bool Bu::Fifo::isReadable()
{
	return (iIn>-1);
}

bool Bu::Fifo::isWritable()
{
	return (iOut>-1);
}

bool Bu::Fifo::isSeekable()
{
	return false;
}

bool Bu::Fifo::isBlocking()
{
#ifndef WIN32
	return ((fcntl( iIn, F_GETFL, 0 )&O_NONBLOCK) == O_NONBLOCK);
#else
	#warning Bu::Fifo::isBlocking IS A STUB for WIN32!!!!
#endif	
}

void Bu::Fifo::setBlocking( bool bBlocking )
{
#ifndef WIN32
	if( bBlocking )
		fcntl( iIn, F_SETFL, fcntl( iIn, F_GETFL, 0 )&(~O_NONBLOCK) );
	else
		fcntl( iIn, F_SETFL, fcntl( iIn, F_GETFL, 0 )|O_NONBLOCK );
#else
	#warning Bu::Fifo::setBlocking IS A STUB for WIN32!!!!
#endif	
}

void Bu::Fifo::flush()
{
}

bool Bu::Fifo::isOpen()
{
	return (iIn > -1) || (iOut > -1);
}


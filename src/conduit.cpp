/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/conduit.h"

Bu::Conduit::Conduit( int iBlockSize ) :
	qb( iBlockSize ),
	bBlocking( true ),
	bOpen( true )
{
}

Bu::Conduit::~Conduit()
{
}

void Bu::Conduit::close()
{
	im.lock();
//	qb.close();
	bOpen = false;
	
	cBlock.signal();
	im.unlock();
}

#include <stdio.h>
Bu::size Bu::Conduit::read( void *pBuf, Bu::size nBytes )
{
	if( !isOpen() )
	{
		return 0;
	}
	im.lock();
	if( bBlocking )
	{
		im.unlock();
		cBlock.lock();
		for(;;)
		{
			im.lock();
			if( qb.getSize() == 0 && bOpen == false )
			{
				im.unlock();
				cBlock.unlock();
				return 0;
			}
			else if( qb.getSize() > 0 )
			{
				im.unlock();
				break;
			}
			im.unlock();

			cBlock.wait();
		}

		im.lock();
		Bu::size iRet = qb.read( pBuf, nBytes );
		im.unlock();

		cBlock.unlock();
		return iRet;
	}
	else
	{
		Bu::size iRet = qb.read( pBuf, nBytes );
		im.unlock();

		return iRet;
	}
}

Bu::size Bu::Conduit::peek( void *pBuf, Bu::size nBytes )
{
	im.lock();
	Bu::size iRet = qb.peek( pBuf, nBytes );
	im.unlock();

	return iRet;
}

Bu::size Bu::Conduit::peek( void *pBuf, Bu::size nBytes, Bu::size nSkip )
{
	im.lock();
	Bu::size iRet = qb.peek( pBuf, nBytes, nSkip );
	im.unlock();

	return iRet;
}

Bu::size Bu::Conduit::write( const void *pBuf, Bu::size nBytes )
{
	im.lock();
	if( bOpen == false )
	{
		im.unlock();
		return 0;
	}
	Bu::size sRet = qb.write( pBuf, nBytes );
	cBlock.signal();
	im.unlock();

	return sRet;
}

Bu::size Bu::Conduit::tell()
{
	im.lock();
	Bu::size sRet = qb.tell();
	im.unlock();
	return sRet;
}

void Bu::Conduit::seek( Bu::size )
{
}

void Bu::Conduit::setPos( Bu::size )
{
}

void Bu::Conduit::setPosEnd( Bu::size )
{
}

bool Bu::Conduit::isEos()
{
	im.lock();
	bool bRet = qb.isEos();
	im.unlock();
	return bRet;
}

bool Bu::Conduit::isOpen()
{
	im.lock();
	bool bRet = bOpen || (qb.getSize() > 0);
	im.unlock();
	return bRet;
}

void Bu::Conduit::flush()
{
}

bool Bu::Conduit::canRead()
{
	im.lock();
	bool bRet = qb.canRead();
	im.unlock();
	return bRet;
}

bool Bu::Conduit::canWrite()
{
	im.lock();
	bool bRet = qb.canWrite();
	im.unlock();
	return bRet;
}

bool Bu::Conduit::isReadable()
{
	im.lock();
	bool bRet = qb.isReadable();
	im.unlock();
	return bRet;
}

bool Bu::Conduit::isWritable()
{
	im.lock();
	bool bRet = qb.isWritable();
	im.unlock();
	return bRet;
}

bool Bu::Conduit::isSeekable()
{
	im.lock();
	bool bRet = qb.isSeekable();
	im.unlock();
	return bRet;
}

bool Bu::Conduit::isBlocking()
{
	im.lock();
	bool bRet = bBlocking;
	im.unlock();
	return bRet;
}

void Bu::Conduit::setBlocking( bool bBlocking )
{
	im.lock();
	this->bBlocking = bBlocking;
	im.unlock();
}

void Bu::Conduit::setSize( Bu::size )
{
}

Bu::size Bu::Conduit::getSize() const
{
	im.lock();
	Bu::size sRet = qb.getSize();
	im.unlock();
	return sRet;
}

Bu::size Bu::Conduit::getBlockSize() const
{
	im.lock();
	Bu::size sRet = qb.getBlockSize();
	im.unlock();
	return sRet;
}

Bu::String Bu::Conduit::getLocation() const
{
	im.lock();
	Bu::String sRet = qb.getLocation();
	im.unlock();
	return sRet;
}


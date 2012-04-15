/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/streamstack.h"

Bu::StreamStack::StreamStack()
{
}

Bu::StreamStack::StreamStack( Bu::Stream *pStream )
{
	lFilts.prepend( pStream );
}

Bu::StreamStack::~StreamStack()
{
	clear();
}

bool Bu::StreamStack::isEmpty()
{
	return lFilts.isEmpty();
}

bool Bu::StreamStack::hasStream()
{
	return !lFilts.isEmpty();
}

void Bu::StreamStack::setStream( Bu::Stream *pStream )
{
	if( !lFilts.isEmpty() )
		throw Bu::ExceptionBase("There is already a stream set.");

	lFilts.prepend( pStream );
}

void Bu::StreamStack::clear()
{
	for( FilterList::iterator i = lFilts.begin(); i; i++ )
	{
		delete *i;
	}

	lFilts.clear();
}

void Bu::StreamStack::popFilter()
{
	if( lFilts.isEmpty() )
		return;

	delete lFilts.first();
	lFilts.erase( lFilts.begin() );
}

Bu::Stream *Bu::StreamStack::getTop()
{
	checkStack();

	return lFilts.first();
}

Bu::Stream *Bu::StreamStack::getStream()
{
	checkStack();

	return lFilts.last();
}

void Bu::StreamStack::close()
{
	checkStack();

	lFilts.first()->close();
}

Bu::size Bu::StreamStack::read( void *pBuf, Bu::size nBytes )
{
	checkStack();

	return lFilts.first()->read( pBuf, nBytes );
}

Bu::size Bu::StreamStack::write( const void *pBuf, Bu::size nBytes )
{
	checkStack();

	return lFilts.first()->write( pBuf, nBytes );
}

Bu::size Bu::StreamStack::write( const Bu::String &sBuf )
{
	checkStack();

	return lFilts.first()->write( sBuf );
}

Bu::size Bu::StreamStack::tell()
{
	checkStack();

	return lFilts.first()->tell();
}

void Bu::StreamStack::seek( Bu::size offset )
{
	checkStack();

	lFilts.first()->seek( offset );
}

void Bu::StreamStack::setPos( Bu::size pos )
{
	checkStack();

	lFilts.first()->setPos( pos );
}

void Bu::StreamStack::setPosEnd( Bu::size pos )
{
	checkStack();

	lFilts.first()->setPosEnd( pos );
}

bool Bu::StreamStack::isEos()
{
	checkStack();

	return lFilts.first()->isEos();
}

bool Bu::StreamStack::isOpen()
{
	checkStack();

	return lFilts.first()->isOpen();
}

void Bu::StreamStack::flush()
{
	checkStack();

	lFilts.first()->flush();
}

bool Bu::StreamStack::canRead()
{
	checkStack();

	return lFilts.first()->canRead();
}

bool Bu::StreamStack::canWrite()
{
	checkStack();

	return lFilts.first()->canWrite();
}

bool Bu::StreamStack::isReadable()
{
	checkStack();

	return lFilts.first()->isReadable();
}

bool Bu::StreamStack::isWritable()
{
	checkStack();

	return lFilts.first()->isWritable();
}

bool Bu::StreamStack::isSeekable()
{
	checkStack();

	return lFilts.first()->isSeekable();
}

bool Bu::StreamStack::isBlocking()
{
	checkStack();

	return lFilts.first()->isBlocking();
}

void Bu::StreamStack::setBlocking( bool bBlocking )
{
	checkStack();

	lFilts.first()->setBlocking( bBlocking );
}

void Bu::StreamStack::setSize( Bu::size iSize )
{
	checkStack();

	lFilts.first()->setSize( iSize );
}

Bu::size Bu::StreamStack::getSize() const
{
	checkStack();
	
	return lFilts.first()->getSize();
}

Bu::size Bu::StreamStack::getBlockSize() const
{
	checkStack();

	return lFilts.first()->getBlockSize();
}

Bu::String Bu::StreamStack::getLocation() const
{
	checkStack();

	return lFilts.first()->getLocation();
}

inline void Bu::StreamStack::checkStack() const
{
	if( lFilts.isEmpty() )
		throw Bu::ExceptionBase("StreamStack is empty.");
}


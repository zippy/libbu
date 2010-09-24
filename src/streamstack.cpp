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

size_t Bu::StreamStack::read( void *pBuf, size_t nBytes )
{
	checkStack();

	return lFilts.first()->read( pBuf, nBytes );
}

size_t Bu::StreamStack::write( const void *pBuf, size_t nBytes )
{
	checkStack();

	return lFilts.first()->write( pBuf, nBytes );
}

size_t Bu::StreamStack::write( const Bu::FString &sBuf )
{
	checkStack();

	return lFilts.first()->write( sBuf );
}

long Bu::StreamStack::tell()
{
	checkStack();

	return lFilts.first()->tell();
}

void Bu::StreamStack::seek( long offset )
{
	checkStack();

	lFilts.first()->seek( offset );
}

void Bu::StreamStack::setPos( long pos )
{
	checkStack();

	lFilts.first()->setPos( pos );
}

void Bu::StreamStack::setPosEnd( long pos )
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

void Bu::StreamStack::setSize( long iSize )
{
	checkStack();

	lFilts.first()->setSize( iSize );
}

inline void Bu::StreamStack::checkStack()
{
	if( lFilts.isEmpty() )
		throw Bu::ExceptionBase("StreamStack is empty.");
}


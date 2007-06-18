#include "bu/filter.h"

Bu::Filter::Filter( Bu::Stream &rNext ) :
	rNext( rNext )
{
}

Bu::Filter::~Filter()
{
	//printf("-> Bu::Filter::~Filter()\n");		
}
/*
void Bu::Filter::start()
{
	printf("-> Bu::Filter::start()\n");
}

void Bu::Filter::stop()
{
}*/

void Bu::Filter::close()
{
	stop();
	rNext.close();
}

long Bu::Filter::tell()
{
	return rNext.tell();
}

void Bu::Filter::seek( long offset )
{
	rNext.seek( offset );
}

void Bu::Filter::setPos( long pos )
{
	rNext.setPos( pos );
}

void Bu::Filter::setPosEnd( long pos )
{
	rNext.setPosEnd( pos );
}

bool Bu::Filter::isEOS()
{
	return rNext.isEOS();
}

bool Bu::Filter::isOpen()
{
	return rNext.isOpen();
}

bool Bu::Filter::canRead()
{
	return rNext.canRead();
}

bool Bu::Filter::canWrite()
{
	return rNext.canWrite();
}

bool Bu::Filter::isReadable()
{
	return rNext.isReadable();
}

bool Bu::Filter::isWritable()
{
	return rNext.isWritable();
}

bool Bu::Filter::isSeekable()
{
	return rNext.isSeekable();
}

bool Bu::Filter::isBlocking()
{
	return rNext.isBlocking();
}

void Bu::Filter::setBlocking( bool bBlocking )
{
	rNext.setBlocking( bBlocking );
}

void Bu::Filter::flush()
{
	rNext.flush();
}


/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "file.h"
#include "exceptions.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

Bu::File::File( const char *sName, const char *sFlags )
{
	fh = fopen( sName, sFlags );
	if( fh == NULL )
	{
		throw Bu::FileException( errno, "%s: %s", strerror(errno), sName );
	}
}

Bu::File::File( const Bu::FString &sName, const char *sFlags )
{
	fh = fopen( sName.getStr(), sFlags );
	if( fh == NULL )
	{
		throw Bu::FileException( errno, "%s: %s", strerror(errno), sName.getStr() );
	}
}

Bu::File::File( int fd, const char *sFlags )
{
	fh = fdopen( fd, sFlags );
}

Bu::File::~File()
{
	close();
}

void Bu::File::close()
{
	if( fh )
	{
		fclose( fh );
		fh = NULL;
	}
}

size_t Bu::File::read( void *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	int nAmnt = fread( pBuf, 1, nBytes, fh );

	//if( nAmnt == 0 )
	//	throw FileException("End of file.");

	return nAmnt;
}

size_t Bu::File::write( const void *pBuf, size_t nBytes )
{
	if( !fh )
		throw FileException("File not open.");

	return fwrite( pBuf, 1, nBytes, fh );
}

long Bu::File::tell()
{
	if( !fh )
		throw FileException("File not open.");

	return ftell( fh );
}

void Bu::File::seek( long offset )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, offset, SEEK_CUR );
}

void Bu::File::setPos( long pos )
{
	if( !fh )
		throw FileException("File not open.");

	fseek( fh, pos, SEEK_SET );
}

void Bu::File::setPosEnd( long pos )
{
	if( !fh )
		throw FileException("File not open.");
	
	fseek( fh, pos, SEEK_END );
}

bool Bu::File::isEOS()
{
	return feof( fh );
}

bool Bu::File::canRead()
{
	return true;
}

bool Bu::File::canWrite()
{
	return true;
}

bool Bu::File::isReadable()
{
	return true;
}

bool Bu::File::isWritable()
{
	return true;
}

bool Bu::File::isSeekable()
{
	return true;
}

bool Bu::File::isBlocking()
{
	return true;
}

void Bu::File::setBlocking( bool bBlocking )
{
	return;
}

#ifndef WIN32
void Bu::File::truncate( long nSize )
{
	ftruncate( fileno( fh ), nSize );
}

void Bu::File::chmod( mode_t t )
{
	fchmod( fileno( fh ), t );
}
#endif

void Bu::File::flush()
{
	fflush( fh );
}

bool Bu::File::isOpen()
{
	return (fh != NULL);
}


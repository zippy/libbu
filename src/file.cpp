/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/file.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // for mkstemp

namespace Bu { subExceptionDef( FileException ) }

Bu::File::File( const Bu::FString &sName, int iFlags ) :
	fd( -1 ),
	bEos( true )
{
	fd = ::open( sName.getStr(), getPosixFlags( iFlags ), 0666 );
	if( fd < 0 )
	{
		throw Bu::FileException( errno, "%s: %s",
			strerror(errno), sName.getStr() );
	}
	bEos = false;
}

Bu::File::File( int fd ) :
	fd( fd )
{
	bEos = false;
}

Bu::File::~File()
{
	close();
}

void Bu::File::close()
{
	if( fd >= 0 )
	{
		if( ::close( fd ) )
		{
			throw Bu::FileException( errno, "%s",
				strerror(errno) );
		}
		fd = -1;
		bEos = true;
	}
}

size_t Bu::File::read( void *pBuf, size_t nBytes )
{
	if( fd < 0 )
		throw FileException("File not open.");

	ssize_t iRead = ::read( fd, pBuf, nBytes );
	if( iRead == 0 )
		bEos = true;
	else if( iRead == -1 && errno == EAGAIN )
		return 0;
	else if( iRead < 0 )
		throw FileException( errno, "%s", strerror( errno ) );
	return iRead;
}

size_t Bu::File::write( const void *pBuf, size_t nBytes )
{
	if( fd < 0 )
		throw FileException("File not open.");

	ssize_t iWrote = ::write( fd, pBuf, nBytes );
	if( iWrote < 0 )
		throw FileException( errno, "%s", strerror( errno ) );
	return iWrote;
}

long Bu::File::tell()
{
	if( fd < 0 )
		throw FileException("File not open.");

	return lseek( fd, 0, SEEK_CUR );
}

void Bu::File::seek( long offset )
{
	if( fd < 0 )
		throw FileException("File not open.");

	lseek( fd, offset, SEEK_CUR );
	bEos = false;
}

void Bu::File::setPos( long pos )
{
	if( fd < 0 )
		throw FileException("File not open.");

	lseek( fd, pos, SEEK_SET );
	bEos = false;
}

void Bu::File::setPosEnd( long pos )
{
	if( fd < 0 )
		throw FileException("File not open.");
	
	lseek( fd, pos, SEEK_END );
	bEos = false;
}

bool Bu::File::isEos()
{
	return bEos;
}

bool Bu::File::canRead()
{
#ifdef WIN32
	return true;
#else
	int iMode = fcntl( fd, F_GETFL, 0 )&O_ACCMODE;
	if( iMode == O_RDONLY || iMode == O_RDWR )
		return true;
	return false;
#endif
}

bool Bu::File::canWrite()
{
#ifdef WIN32
	return true;
#else
	int iMode = fcntl( fd, F_GETFL, 0 )&O_ACCMODE;
	if( iMode == O_WRONLY || iMode == O_RDWR )
		return true;
	return false;
#endif
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
#ifdef WIN32
	fprintf(stderr, "STUB: Bu::File::setBlocking\n");
#else
	if( bBlocking )
		fcntl( 
			fd,
			F_SETFL, fcntl( fd, F_GETFL, 0 )&(~O_NONBLOCK)
			);
	else
		fcntl( 
			fd,
			F_SETFL, fcntl( fd, F_GETFL, 0 )|O_NONBLOCK
			);
#endif
}

#ifndef WIN32
Bu::File Bu::File::tempFile( Bu::FString &sName )
{
	int afh_d = mkstemp( sName.getStr() );

	return Bu::File( afh_d );
}
void Bu::File::truncate( long nSize )
{
	ftruncate( fd, nSize );
}

void Bu::File::chmod( mode_t t )
{
	fchmod( fd, t );
}
#endif

void Bu::File::flush()
{
	// There is no flushing with direct I/O...
	//fflush( fh );
}

bool Bu::File::isOpen()
{
	return (fd > -1);
}

int Bu::File::getPosixFlags( int iFlags )
{
	int iRet = 0;
	switch( (iFlags&ReadWrite) )
	{
		// According to posix, O_RDWR is not necesarilly O_RDONLY|O_WRONLY, so
		// lets be proper and use the right value in the right place.
		case Read:		iRet = O_RDONLY; break;
		case Write:		iRet = O_WRONLY; break;
		case ReadWrite:	iRet = O_RDWR; break;
		default:
			throw FileException(
				"You must specify Read, Write, or both when opening a file.");
	}

	if( (iFlags&Create) )
		iRet |= O_CREAT;
	if( (iFlags&Append) )
		iRet |= O_APPEND;
	if( (iFlags&Truncate) )
		iRet |= O_TRUNC;
#ifndef WIN32
	if( (iFlags&NonBlock) )
		iRet |= O_NONBLOCK;
#endif
	if( (iFlags&Exclusive) == Exclusive )
		iRet |= O_EXCL;

#ifdef O_BINARY
	iRet |= O_BINARY;
#endif

	return iRet;
}


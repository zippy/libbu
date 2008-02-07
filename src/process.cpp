/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "process.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>

Bu::Process::Process( const char *sName, char *const argv[] )
{
	gexec( sName, argv );
}

Bu::Process::Process( const char *sName, const char *argv, ...)
{
	int iCnt = 0;
	va_list ap;
	va_start( ap, argv );
	for(; va_arg( ap, const char *); iCnt++ );
	va_end( ap );

	char const **list = new char const *[iCnt+2];
	va_start( ap, argv );
	list[0] = argv;
	for( int j = 1; j <= iCnt; j++ )
	{
		list[j] = va_arg( ap, const char *);
	}
	list[iCnt+1] = NULL;
	va_end( ap );

	gexec( sName, (char *const *)list );
	delete[] list;
}

Bu::Process::~Process()
{
}

void Bu::Process::gexec( const char *sName, char *const argv[] )
{
	int iaStdIn[2];
	int iaStdOut[2];
//	int iaStdErr[2];
	pipe( iaStdIn );
	pipe( iaStdOut );
//	pipe( iaStdErr );

	iStdIn = iaStdIn[1];
	iStdOut = iaStdOut[0];
//	iStdErr = iaStdErr[0];

//	fcntl( iStdOut, F_SETFL, fcntl( iStdOut, F_GETFL, 0 )|O_NONBLOCK );

	iPid = fork();
	if( iPid == 0 )
	{
		::close( iaStdIn[1] );
		::close( iaStdOut[0] );
//		::close( iaStdErr[0] );
		dup2( iaStdIn[0], 0 );
		dup2( iaStdOut[1], 1 );
//		dup2( iaStdErr[1], 2 );
		execvp( sName, argv );
	}
	::close( iaStdIn[0] );
	::close( iaStdOut[1] );
//	::close( iaStdErr[1] );
}

void Bu::Process::close()
{
}

size_t Bu::Process::read( void *pBuf, size_t nBytes )
{
	size_t iTotal = 0;
	for(;;)
	{
		size_t iRet = ::read( iStdOut, (char *)pBuf+iTotal, nBytes-iTotal );
		if( iRet == 0 )
			return iTotal;
		iTotal += iRet;
		if( iTotal == nBytes )
			return iTotal;
	}
	/*
	size_t iTotal = 0;
	fd_set rfs;
	FD_ZERO( &rfs );
	for(;;)
	{
		if( waitpid( iPid, NULL, WNOHANG ) )
		{
			printf("!!!wait failed!\n");
			size_t iRet = ::read( iStdOut, (char *)pBuf+iTotal,
					nBytes-iTotal );
			iTotal += iRet;
			return iTotal;
		}

		FD_SET( iStdOut, &rfs );
		select( iStdOut+1, &rfs, NULL, &rfs, NULL );
		size_t iRet = ::read( iStdOut, (char *)pBuf+iTotal, nBytes-iTotal );
		printf("--read=%d / %d--\n", iRet, iTotal+iRet );
		iTotal += iRet;
		if( iTotal == nBytes )
			return iTotal;
	}
	*/
}

size_t Bu::Process::readErr( void *pBuf, size_t nBytes )
{
	return ::read( iStdErr, pBuf, nBytes );
}

size_t Bu::Process::write( const void *pBuf, size_t nBytes )
{
	return ::write( iStdIn, pBuf, nBytes );
}

long Bu::Process::tell()
{
	return 0;
}

void Bu::Process::seek( long offset )
{
}

void Bu::Process::setPos( long pos )
{
}

void Bu::Process::setPosEnd( long pos )
{
}

bool Bu::Process::isEOS()
{
	return false;
}

bool Bu::Process::isOpen()
{
	return true;
}

void Bu::Process::flush()
{
}

bool Bu::Process::canRead()
{
	return true;
}

bool Bu::Process::canWrite()
{
	return true;
}

bool Bu::Process::isReadable()
{
	return true;
}

bool Bu::Process::isWritable()
{
	return true;
}

bool Bu::Process::isSeekable()
{
	return false;
}

bool Bu::Process::isBlocking()
{
	return true;
}

void Bu::Process::setBlocking( bool bBlocking )
{
}


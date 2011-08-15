/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/process.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/select.h>

#include "bu/config.h"

Bu::Process::Process( Flags eFlags, const char *sName, char *const argv[] ) :
	iStdIn( -1 ),
	iStdOut( -1 ),
	iStdErr( -1 ),
	iPid( 0 ),
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true )
{
	gexec( eFlags, sName, argv );
}

Bu::Process::Process( Flags eFlags, const char *sName, const char *argv, ...) :
	iStdIn( -1 ),
	iStdOut( -1 ),
	iStdErr( -1 ),
	iPid( 0 ),
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true )
{
	int iCnt = 0;
	va_list ap;
	va_start( ap, argv );
	for(; va_arg( ap, const char *); iCnt++ ) { }
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

	gexec( eFlags, sName, (char *const *)list );
	delete[] list;
}

Bu::Process::Process( Flags eFlags, const Bu::Process::Options &opt, const char *sName, char *const argv[] ) :
	iStdIn( -1 ),
	iStdOut( -1 ),
	iStdErr( -1 ),
	iPid( 0 ),
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true ),
	opt( opt )
{
	gexec( eFlags, sName, argv );
}

Bu::Process::Process( Flags eFlags, const Bu::Process::Options &opt, const char *sName, const char *argv, ...) :
	iStdIn( -1 ),
	iStdOut( -1 ),
	iStdErr( -1 ),
	iPid( 0 ),
	iProcStatus( 0 ),
	bBlocking( true ),
	bStdOutEos( true ),
	bStdErrEos( true ),
	opt( opt )
{
	int iCnt = 0;
	va_list ap;
	va_start( ap, argv );
	for(; va_arg( ap, const char *); iCnt++ ) { }
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

	gexec( eFlags, sName, (char *const *)list );
	delete[] list;
}

Bu::Process::~Process()
{
	close();
}

void Bu::Process::wait()
{
	close();
}

void Bu::Process::gexec( Flags eFlags, const char *sName, char *const argv[] )
{
	int iaStdIn[2];
	int iaStdOut[2];
	int iaStdErr[2];
	pipe( iaStdIn );
	if( eFlags & StdOut )
	{
		pipe( iaStdOut );
		iStdOut = iaStdOut[0];
		bStdOutEos = false;
	}
	if( eFlags & StdErr )
	{
		pipe( iaStdErr );
		iStdErr = iaStdErr[0];
		bStdErrEos = false;
	}

	iStdIn = iaStdIn[1];

//	fcntl( iStdOut, F_SETFL, fcntl( iStdOut, F_GETFL, 0 )|O_NONBLOCK );

	iPid = fork();
	if( iPid == 0 )
	{
		::close( iaStdIn[1] );
		dup2( iaStdIn[0], 0 );
		if( eFlags & StdOut )
		{
			::close( iaStdOut[0] );
			dup2( iaStdOut[1], 1 );
		}
		if( eFlags & StdErr )
		{
			::close( iaStdErr[0] );
			dup2( iaStdErr[1], 2 );
		}
		if( (opt.eFlags&Options::SetUid) )
		{
			setuid( opt.iUid );
		}
		execvp( sName, argv );
		throw Bu::ExceptionBase("Hey, execvp failed!");
	}
	::close( iaStdIn[0] );
	if( eFlags & StdOut )
		::close( iaStdOut[1] );
	if( eFlags & StdErr )
		::close( iaStdErr[1] );
}

void Bu::Process::close()
{
	if( iPid )
	{
		::close( iStdIn );
		if( iStdErr > -1 )
			::close( iStdOut );
		if( iStdErr > -1 )
			::close( iStdErr );
		waitpid( iPid, &iProcStatus, 0 );
		iPid = 0;
	}
}

Bu::size Bu::Process::read( void *pBuf, Bu::size nBytes )
{
	if( bStdOutEos )
		return 0;
	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( iStdOut, &rfds );
	struct timeval tv = {0, 0};
	if( ::bu_select( iStdOut+1, &rfds, NULL, NULL, &tv ) < 0 )
		throw Bu::ExceptionBase( strerror( errno ) );
	if( FD_ISSET( iStdOut, &rfds ) || bBlocking )
	{
		Bu::size nRead = TEMP_FAILURE_RETRY( ::read( iStdOut, pBuf, nBytes ) );
		if( nRead == 0 )
		{
			bStdOutEos = true;
			checkClose();
			return 0;
		}
		if( nRead < 0 )
		{
			if( errno == EAGAIN )
				return 0;
			throw Bu::ExceptionBase( strerror( errno ) );
		}
		return nRead;
	}
	return 0;
}

Bu::size Bu::Process::readErr( void *pBuf, Bu::size nBytes )
{
	if( bStdErrEos )
		return 0;
	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( iStdErr, &rfds );
	struct timeval tv = {0, 0};
	if( ::bu_select( iStdErr+1, &rfds, NULL, NULL, &tv ) < 0 )
		throw Bu::ExceptionBase( strerror( errno ) );
	if( FD_ISSET( iStdErr, &rfds ) || bBlocking )
	{
		Bu::size nRead = TEMP_FAILURE_RETRY( ::read( iStdErr, pBuf, nBytes ) );
		if( nRead == 0 )
		{
			bStdErrEos = true;
			checkClose();
			return 0;
		}
		if( nRead < 0 )
		{
			if( errno == EAGAIN )
				return 0;
			throw Bu::ExceptionBase( strerror( errno ) );
		}
		return nRead;
	}
	return 0;
}

Bu::size Bu::Process::write( const void *pBuf, Bu::size nBytes )
{
	return TEMP_FAILURE_RETRY( ::write( iStdIn, pBuf, nBytes ) );
}

Bu::size Bu::Process::tell()
{
	return 0;
}

void Bu::Process::seek( Bu::size )
{
}

void Bu::Process::setPos( Bu::size )
{
}

void Bu::Process::setPosEnd( Bu::size )
{
}

bool Bu::Process::isEos()
{
	return (iPid == 0);
}

bool Bu::Process::isOpen()
{
	return (iPid != 0);
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
	if( bBlocking )
	{
		if( !bStdOutEos )
			fcntl( iStdOut, F_SETFL, fcntl(iStdOut,F_GETFL,0 )&(~O_NONBLOCK) );
		if( !bStdErrEos )
			fcntl( iStdErr, F_SETFL, fcntl(iStdErr,F_GETFL,0 )&(~O_NONBLOCK) );
	}
	else
	{
		if( !bStdOutEos )
			fcntl( iStdOut, F_SETFL, fcntl( iStdOut, F_GETFL, 0 )|O_NONBLOCK );
		if( !bStdErrEos )
			fcntl( iStdErr, F_SETFL, fcntl( iStdErr, F_GETFL, 0 )|O_NONBLOCK );
	}
	this->bBlocking = bBlocking;
}

void Bu::Process::setSize( Bu::size )
{
}

Bu::size Bu::Process::getBlockSize() const
{
	return 0;
}

Bu::size Bu::Process::getSize() const
{
	return 0;
}

Bu::String Bu::Process::getLocation() const
{
	return "";
}

void Bu::Process::select( bool &bStdOut, bool &bStdErr )
{
	fd_set rfds;
	FD_ZERO( &rfds );
	if( !bStdOutEos )
		FD_SET( iStdOut, &rfds );
	if( !bStdErrEos )
		FD_SET( iStdErr, &rfds );
	if( ::bu_select( iStdErr+1, &rfds, NULL, NULL, NULL ) < 0 )	
		throw Bu::ExceptionBase( strerror( errno ) );

	if( FD_ISSET( iStdOut, &rfds ) )
		bStdOut = true;
	else
		bStdOut = false;

	if( FD_ISSET( iStdErr, &rfds ) )
		bStdErr = true;
	else
		bStdErr = false;
}

bool Bu::Process::isRunning()
{
	if( waitpid( iPid, NULL, WNOHANG ) == iPid )
		close();
	return iPid != 0;
}

void Bu::Process::ignoreStdErr()
{
	if( iStdErr == -1 )
		return;
	::close( iStdErr );
	iStdErr = -1;
	bStdErrEos = true;
}

pid_t Bu::Process::getPid()
{
	return iPid;
}

bool Bu::Process::childExited()
{
	return WIFEXITED( iProcStatus );
}

int Bu::Process::childExitStatus()
{
	return WEXITSTATUS( iProcStatus );
}

bool Bu::Process::childSignaled()
{
	return WIFSIGNALED( iProcStatus );
}

int Bu::Process::childSignal()
{
	return WTERMSIG( iProcStatus );
}

bool Bu::Process::childCoreDumped()
{
	return WCOREDUMP( iProcStatus );
}

void Bu::Process::checkClose()
{
	if( bStdOutEos && bStdErrEos )
	{
		close();
	}
}


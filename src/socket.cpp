/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include "socket.h"
#include "exceptions.h"
#include "osx_compatibility.h"

#define RBS (1024*2)

Bu::Socket::Socket( int nSocket ) :
	nSocket( nSocket ),
	bActive( true )
{
	setAddress();
}

Bu::Socket::Socket( const Bu::FString &sAddr, int nPort, int nTimeout )
{
	struct sockaddr_in xServerName;
	bActive = false;
     
	/* Create the socket. */
	nSocket = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( nSocket < 0 )
	{
		throw ExceptionBase("Couldn't create socket.\n");
	}

	// These lines set the socket to non-blocking, a good thing?
	int flags;
	flags = fcntl(nSocket, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(nSocket, F_SETFL, flags) < 0)
	{
		throw ExceptionBase("Couldn't set socket options.\n");
	}
     
	/* Connect to the server. */
	//printf("Resolving hostname (%s)...\n", sAddr );
	{
		struct hostent *hostinfo;
     
		xServerName.sin_family = AF_INET;
		xServerName.sin_port = htons( nPort );
		hostinfo = gethostbyname( sAddr.getStr() );
		if (hostinfo == NULL)
		{
			throw ExceptionBase("Couldn't resolve hostname.\n");
		}
		xServerName.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	}

	//printf("Making actual connection...");
	//fflush( stdout );
	connect(
		nSocket,
		(struct sockaddr *)&xServerName,
		sizeof(xServerName)
		);
	//printf("Connected.\n");
	
	bActive = true;

	if( nTimeout > 0 )
	{
		fd_set rfds, wfds, efds;
		int retval;
		
		FD_ZERO(&rfds);
		FD_SET(nSocket, &rfds);
		FD_ZERO(&wfds);
		FD_SET(nSocket, &wfds);
		FD_ZERO(&efds);
		FD_SET(nSocket, &efds);

		struct timeval tv;
		tv.tv_sec = nTimeout;
		tv.tv_usec = 0;
		
		retval = select( nSocket+1, &rfds, &wfds, &efds, &tv );

		if( retval == 0 )
		{
			close();
			throw ExceptionBase("Connection timeout.\n");
		}
	}
	setAddress();
}

Bu::Socket::~Socket()
{
}

void Bu::Socket::close()
{
	if( bActive )
	{
		fsync( nSocket );
		::close( nSocket );
	}
	bActive = false;
}

/*
void Bu::Socket::read()
{
	char buffer[RBS];
	int nbytes;
	int nTotalRead=0;

	for(;;)
	{
		//memset( buffer, 0, RBS );

		nbytes = ::read( nSocket, buffer, RBS );
		if( nbytes < 0 && errno != 0 && errno != EAGAIN )
		{
			//printf("errno: %d, %s\n", errno, strerror( errno ) );
			//perror("readInput");
			throw ConnectionException(
				excodeReadError,
				"Read error: %s",
				strerror( errno )
				);
		}
		else
		{
			if( nbytes <= 0 )
				break;
			nTotalRead += nbytes;
			sReadBuf.append( buffer, nbytes );
			if( nbytes < RBS )
			{
				break;
			}

			// New test, if data is divisible by RBS bytes on some libs the
			// read could block, this keeps it from happening.
			{
				fd_set rfds;
				FD_ZERO(&rfds);
				FD_SET(nSocket, &rfds);
				struct timeval tv = { 0, 0 };
				int retval = select( nSocket+1, &rfds, NULL, NULL, &tv );
				if( retval == -1 )
					throw ConnectionException(
						excodeBadReadError,
						"Bad Read error"
						);
				if( !FD_ISSET( nSocket, &rfds ) )
					break;
			}
		}
	}
}*/

size_t Bu::Socket::read( void *pBuf, size_t nBytes )
{
	int nRead = TEMP_FAILURE_RETRY( ::read( nSocket, pBuf, nBytes ) );
	if( nRead < 0 )
	{
		throw ConnectionException( excodeReadError, strerror(errno) );
	}
	return nRead;
}

size_t Bu::Socket::read( void *pBuf, size_t nBytes,
		uint32_t nSec, uint32_t nUSec )
{
	struct timeval tv, nt, ct;
	size_t nRead = 0;
	
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(nSocket, &rfds);

	gettimeofday( &nt, NULL );
	nt.tv_sec += nSec;
	nt.tv_usec += nUSec;

	for(;;)
	{
		tv.tv_sec = nSec;
		tv.tv_usec = nUSec;
		select( nSocket+1, &rfds, NULL, NULL, &tv );
		nRead += read( ((char *)pBuf)+nRead, nBytes-nRead );
		if( nRead >= nBytes )
			break;
		gettimeofday( &ct, NULL );
		if( (ct.tv_sec > nt.tv_sec) ||
			(ct.tv_sec == nt.tv_sec &&
			ct.tv_usec >= nt.tv_usec) )
			break;
	}
	return nRead;
}

size_t Bu::Socket::write( const void *pBuf, size_t nBytes )
{
	int nWrote = TEMP_FAILURE_RETRY( ::write( nSocket, pBuf, nBytes ) );
	if( nWrote < 0 )
	{
		if( errno == EAGAIN ) return 0;
		throw ConnectionException( excodeWriteError, strerror(errno) );
	}
	return nWrote;
}

size_t Bu::Socket::write( const void *pBuf, size_t nBytes, uint32_t nSec, uint32_t nUSec )
{
	struct timeval tv, nt, ct;
	size_t nWrote = 0;
	
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(nSocket, &wfds);

	gettimeofday( &nt, NULL );
	nt.tv_sec += nSec;
	nt.tv_usec += nUSec;

	for(;;)
	{
		tv.tv_sec = nSec;
		tv.tv_usec = nUSec;
		select( nSocket+1, NULL, &wfds, NULL, &tv );
		nWrote += write( ((char *)pBuf)+nWrote, nBytes-nWrote );
		if( nWrote >= nBytes )
			break;
		gettimeofday( &ct, NULL );
		if( (ct.tv_sec > nt.tv_sec) ||
			(ct.tv_sec == nt.tv_sec &&
			ct.tv_usec >= nt.tv_usec) )
			break;
	}
	return nWrote;
}

long Bu::Socket::tell()
{
	throw UnsupportedException();
}

void Bu::Socket::seek( long offset )
{
	throw UnsupportedException();
}

void Bu::Socket::setPos( long pos )
{
	throw UnsupportedException();
}

void Bu::Socket::setPosEnd( long pos )
{
	throw UnsupportedException();
}

bool Bu::Socket::isEOS()
{
	return !bActive;
}

bool Bu::Socket::canRead()
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(nSocket, &rfds);
	struct timeval tv = { 0, 0 };
	int retval = select( nSocket+1, &rfds, NULL, NULL, &tv );
	if( retval == -1 )
		throw ConnectionException(
			excodeBadReadError,
			"Bad Read error"
			);
	if( !FD_ISSET( nSocket, &rfds ) )
		return false;
	return true;
}

bool Bu::Socket::canWrite()
{
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(nSocket, &wfds);
	struct timeval tv = { 0, 0 };
	int retval = select( nSocket+1, NULL, &wfds, NULL, &tv );
	if( retval == -1 )
		throw ConnectionException(
			excodeBadReadError,
			"Bad Read error"
			);
	if( !FD_ISSET( nSocket, &wfds ) )
		return false;
	return true;
}

bool Bu::Socket::isReadable()
{
	return true;
}

bool Bu::Socket::isWritable()
{
	return true;
}

bool Bu::Socket::isSeekable()
{
	return false;
}

bool Bu::Socket::isBlocking()
{
	return false;
}

void Bu::Socket::setBlocking( bool bBlocking )
{
}

void Bu::Socket::flush()
{
}

bool Bu::Socket::isOpen()
{
	return bActive;
}

void Bu::Socket::setAddress()
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	addr.sin_family = AF_INET;
	// getsockname( nSocket, (sockaddr *)(&addr), &len );
	getpeername( nSocket, (sockaddr *)(&addr), &len );
	char buf[150];
	sprintf( buf, "%s", inet_ntoa( addr.sin_addr ) );
	sAddress = buf;
}

Bu::FString Bu::Socket::getAddress() const
{
	return sAddress;
}

Bu::Socket::operator int() const
{
	return nSocket;
}

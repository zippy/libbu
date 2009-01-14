/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
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
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include "socket.h"
#include "osx_compatibility.h"
#include "win32_compatibility.h"

#ifndef WIN32
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <arpa/inet.h>
#else
 #include <Winsock2.h>
#endif

#define RBS (1024*2)

namespace Bu { subExceptionDef( SocketException ) }

Bu::Socket::Socket( int nSocket ) :
	nSocket( nSocket ),
	bActive( true )
{
#ifdef WIN32
	DynamicWinsock32::Winsock2::getInstance();
#endif
	setAddress();
}

Bu::Socket::Socket( const Bu::FString &sAddr, int nPort, int nTimeout )
{
#ifdef WIN32
	DynamicWinsock32::Winsock2::getInstance();
#endif
	bActive = false;
     
	/* Create the socket. */
	nSocket = DYNLOAD socket( PF_INET, SOCK_STREAM, 0 );
	
	if( nSocket < 0 )
	{
		throw ExceptionBase("Couldn't create socket.\n");
	}

	// These lines set the socket to non-blocking, a good thing?
#ifndef WIN32
	int flags;
	flags = fcntl(nSocket, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(nSocket, F_SETFL, flags) < 0)
	{
		throw Bu::SocketException("Couldn't set socket options.\n");
	}
#else
	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	u_long iMode = 1;
	DYNLOAD ioctlsocket(nSocket, FIONBIO, &iMode);
#endif
     
	/* Connect to the server. */
	//printf("Resolving hostname (%s)...\n", sAddr );
	{
		struct addrinfo *pAddr = NULL;
		struct addrinfo aiHints;
		memset( &aiHints, 0, sizeof(addrinfo) );
		aiHints.ai_flags = AI_CANONNAME;
		aiHints.ai_family = AF_INET;
		aiHints.ai_socktype = SOCK_STREAM;
		char ibuf[10]; 
		sprintf( ibuf, "%d", nPort );
     
		if( int ret = DYNLOAD getaddrinfo( sAddr.getStr(), ibuf, &aiHints, &pAddr )
			!= 0 )
		{

		struct addrinfo *pCur = pAddr;
		while( pCur )
		{
			printf("Name: %s\n", pCur->ai_canonname );
			printf("  Flags: %d\n", pCur->ai_flags );
			printf("  Family: %d\n", pCur->ai_family );
			printf("  Socktype: %d\n", pCur->ai_socktype );
			printf("  Protocol: %d\n", pCur->ai_protocol );

			pCur = pCur->ai_next;
		}
			throw Bu::SocketException("Couldn't resolve hostname %s (%s).\n",
				sAddr.getStr(), gai_strerror(ret));
		}

		struct addrinfo *pCur = pAddr;
		while( pCur )
		{
			printf("Name: %s\n", pCur->ai_canonname );
			printf("  Flags: %d\n", pCur->ai_flags );
			printf("  Family: %d\n", pCur->ai_family );
			printf("  Socktype: %d\n", pCur->ai_socktype );
			printf("  Protocol: %d\n", pCur->ai_protocol );

			pCur = pCur->ai_next;
		}

		DYNLOAD connect(
			nSocket,
			pAddr->ai_addr,
			pAddr->ai_addrlen
			);

		sAddress = pAddr->ai_canonname;

		DYNLOAD freeaddrinfo( pAddr );
	}

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
		
		retval = DYNLOAD select( nSocket+1, &rfds, &wfds, &efds, &tv );

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
#ifndef WIN32
		fsync( nSocket );
#endif
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
			throw SocketException(
				SocketException::cRead,
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
					throw SocketException(
						SocketException::cBadRead,
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
#ifdef WIN32
	int nRead = TEMP_FAILURE_RETRY( 
			DYNLOAD recv( nSocket, (char *) pBuf, nBytes, 0 ) );
#else
	int nRead = TEMP_FAILURE_RETRY( ::read( nSocket, pBuf, nBytes ) );
#endif
	if( nRead < 0 )
	{
		if( errno == EAGAIN )
			return 0;
		throw SocketException( SocketException::cRead, strerror(errno) );
	}
	return nRead;
}

size_t Bu::Socket::read( void *pBuf, size_t nBytes,
		uint32_t nSec, uint32_t nUSec )
{
	struct timeval tv;
	size_t nRead = 0;
	
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(nSocket, &rfds);

#ifdef WIN32
	DWORD dwStart = GetTickCount();
	uint64_t uOver = dwStart + ((nUSec / 1000) * (nSec * 1000));
	DWORD dwEnd = uOver>4294967295U?uOver-4294967295U:uOver;
#else
	struct timeval nt, ct;
	gettimeofday( &nt, NULL );
	nt.tv_sec += nSec;
	nt.tv_usec += nUSec;
#endif

	for(;;)
	{
		tv.tv_sec = nSec;
		tv.tv_usec = nUSec;
		DYNLOAD select( nSocket+1, &rfds, NULL, NULL, &tv );
		nRead += read( ((char *)pBuf)+nRead, nBytes-nRead );
		if( nRead >= nBytes )
			break;
#ifdef WIN32
		DWORD dwNow = GetTickCount();
		if( dwNow > dwEnd )
			break;
#else
		gettimeofday( &ct, NULL );
		if( (ct.tv_sec > nt.tv_sec) ||
			(ct.tv_sec == nt.tv_sec &&
			ct.tv_usec >= nt.tv_usec) )
			break;
#endif
	}
	return nRead;
}

size_t Bu::Socket::write( const void *pBuf, size_t nBytes )
{
#ifdef WIN32
	int nWrote = TEMP_FAILURE_RETRY( 
			DYNLOAD send( nSocket, (const char *) pBuf, nBytes, 0 ) );
#else
	int nWrote = TEMP_FAILURE_RETRY( ::write( nSocket, pBuf, nBytes ) );
#endif
	if( nWrote < 0 )
	{
		if( errno == EAGAIN ) return 0;
		throw SocketException( SocketException::cWrite, strerror(errno) );
	}
	return nWrote;
}

size_t Bu::Socket::write( const void *pBuf, size_t nBytes, uint32_t nSec, uint32_t nUSec )
{
	struct timeval tv;
	size_t nWrote = 0;
	
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(nSocket, &wfds);

#ifdef WIN32
	DWORD dwStart = GetTickCount();
	uint64_t uOver = dwStart + ((nUSec / 1000) * (nSec * 1000));
	DWORD dwEnd = uOver>4294967295U?uOver-4294967295U:uOver;
#else
	struct timeval nt, ct;
	gettimeofday( &nt, NULL );
	nt.tv_sec += nSec;
	nt.tv_usec += nUSec;
#endif

	for(;;)
	{
		tv.tv_sec = nSec;
		tv.tv_usec = nUSec;
		DYNLOAD select( nSocket+1, NULL, &wfds, NULL, &tv );
		nWrote += write( ((char *)pBuf)+nWrote, nBytes-nWrote );
		if( nWrote >= nBytes )
			break;
#ifdef WIN32
		DWORD dwNow = GetTickCount();
		if( dwNow > dwEnd )
			break;
#else
		gettimeofday( &ct, NULL );
		if( (ct.tv_sec > nt.tv_sec) ||
			(ct.tv_sec == nt.tv_sec &&
			ct.tv_usec >= nt.tv_usec) )
			break;
#endif
	}
	return nWrote;
}

long Bu::Socket::tell()
{
	throw UnsupportedException();
}

void Bu::Socket::seek( long )
{
	throw UnsupportedException();
}

void Bu::Socket::setPos( long )
{
	throw UnsupportedException();
}

void Bu::Socket::setPosEnd( long )
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
	int retval = DYNLOAD select( nSocket+1, &rfds, NULL, NULL, &tv );
	if( retval == -1 )
		throw SocketException(
			SocketException::cBadRead,
			"Bad Read error"
			);
#ifdef WIN32
	if( !DynamicWinsock2::DYN_FD_ISSET( nSocket, &rfds ) )
#else
	if( !FD_ISSET( nSocket, &rfds ) )
#endif
		return false;
	return true;
}

bool Bu::Socket::canWrite()
{
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(nSocket, &wfds);
	struct timeval tv = { 0, 0 };
	int retval = DYNLOAD select( nSocket+1, NULL, &wfds, NULL, &tv );
	if( retval == -1 )
		throw SocketException(
			SocketException::cBadRead,
			"Bad Read error"
			);
#ifdef WIN32
	if( !DynamicWinsock2::DYN_FD_ISSET( nSocket, &wfds ) )
#else
	if( !FD_ISSET( nSocket, &wfds ) )
#endif
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
#ifndef WIN32
	if( bBlocking )
	{
		fcntl( nSocket, F_SETFL, fcntl( nSocket, F_GETFL, 0 ) & ~O_NONBLOCK );
	}
	else
	{
		fcntl( nSocket, F_SETFL, fcntl( nSocket, F_GETFL, 0 ) | O_NONBLOCK );
	}
#else
	u_long iMode;
	if( bBlocking )
		iMode = 0;
	else
		iMode = 1;
	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	DYNLOAD ioctlsocket(nSocket, FIONBIO, &iMode);
#endif	
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
	DYNLOAD getpeername( nSocket, (sockaddr *)(&addr), &len );
#ifdef WIN32
	DYNLOAD inet_ntoa( sAddress, addr.sin_addr );
#else
	char buf[150];
	sprintf( buf, "%s", inet_ntoa( addr.sin_addr ) );
	sAddress = buf;
#endif
}

Bu::FString Bu::Socket::getAddress() const
{
	return sAddress;
}

Bu::Socket::operator int() const
{
	return nSocket;
}

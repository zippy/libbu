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
#include "bu/socket.h"
#include "bu/osx_compatibility.h"
#include "bu/win32_compatibility.h"
#include "bu/linux_compatibility.h"

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
	Bu::Winsock2::getInstance();
#endif
	setAddress();
}

Bu::Socket::Socket( const Bu::FString &sAddr, int nPort, int nTimeout )
{
#ifdef WIN32
	Bu::Winsock2::getInstance();
#endif
	bActive = false;

	/* Create the socket. */
	nSocket = bu_socket( PF_INET, SOCK_STREAM, 0 );
	
	if( nSocket < 0 )
	{
		throw ExceptionBase("Couldn't create socket.\n");
	}

	setBlocking( false );
     
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
     
		int ret;
		if( (ret = bu_getaddrinfo(
			sAddr.getStr(), ibuf, &aiHints, &pAddr )) != 0 )
		{
			throw Bu::SocketException("Couldn't resolve hostname %s (%s).\n",
				sAddr.getStr(), bu_gai_strerror(ret));
		}

		bu_connect(
			nSocket,
			pAddr->ai_addr,
			pAddr->ai_addrlen
			);

		sAddress = pAddr->ai_canonname;

		bu_freeaddrinfo( pAddr );
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
		
		retval = bu_select( nSocket+1, &rfds, &wfds, &efds, &tv );

		if( retval == 0 )
		{
			close();
			throw ExceptionBase("Connection timeout.\n");
		}
	}
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

size_t Bu::Socket::read( void *pBuf, size_t nBytes )
{
#ifdef WIN32
	int nRead = TEMP_FAILURE_RETRY( 
		bu_recv( nSocket, (char *) pBuf, nBytes, 0 ) );
#else
	int nRead = TEMP_FAILURE_RETRY( ::read( nSocket, pBuf, nBytes ) );
#endif
	if( nRead < 0 )
	{
#ifdef WIN32
		int iWSAError = bu_WSAGetLastError();
		if( iWSAError == WSAEWOULDBLOCK )
			return 0;
#else
		if( errno == EAGAIN )
			return 0;
		throw SocketException( SocketException::cRead, strerror(errno) );
#endif
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
		bu_select( nSocket+1, &rfds, NULL, NULL, &tv );
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
		bu_send( nSocket, (const char *) pBuf, nBytes, 0 ) );
#else
	int nWrote = TEMP_FAILURE_RETRY( ::write( nSocket, pBuf, nBytes ) );
#endif
	if( nWrote < 0 )
	{
#ifdef WIN32
		int iWSAError = bu_WSAGetLastError();
		if( iWSAError == WSAEWOULDBLOCK )
			return 0;
#else
		if( errno == EAGAIN ) return 0;
#endif
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
		bu_select( nSocket+1, NULL, &wfds, NULL, &tv );
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
	int retval = bu_select( nSocket+1, &rfds, NULL, NULL, &tv );
	if( retval == -1 )
		throw SocketException(
			SocketException::cBadRead,
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
	int retval = bu_select( nSocket+1, NULL, &wfds, NULL, &tv );
	if( retval == -1 )
		throw SocketException(
			SocketException::cBadRead,
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
	bu_ioctlsocket(nSocket, FIONBIO, &iMode);
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
	bu_getpeername( nSocket, (sockaddr *)(&addr), &len );
	sAddress = bu_inet_ntoa( addr.sin_addr );
}

Bu::FString Bu::Socket::getAddress() const
{
	return sAddress;
}

Bu::Socket::operator int() const
{
	return nSocket;
}


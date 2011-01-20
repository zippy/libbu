/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
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
#include "bu/tcpsocket.h"

#include "bu/config.h"

#ifndef WIN32
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netdb.h>
 #include <arpa/inet.h>
#else
 #include <Winsock2.h>
#endif

#define RBS (1024*2)

namespace Bu { subExceptionDef( TcpSocketException ) }

Bu::TcpSocket::TcpSocket( int nTcpSocket ) :
	nTcpSocket( nTcpSocket ),
	bActive( true ),
	bBlocking( true )
{
#ifdef WIN32
	Bu::Winsock2::getInstance();
#endif
	setAddress();
}

Bu::TcpSocket::TcpSocket( const Bu::String &sAddr, int nPort, int nTimeout,
		bool bBlocking ) :
	nTcpSocket( 0 ),
	bActive( false ),
	bBlocking( true )
{
#ifdef WIN32
	Bu::Winsock2::getInstance();
#endif

	/* Create the socket. */
	nTcpSocket = bu_socket( PF_INET, SOCK_STREAM, 0 );
	
	if( nTcpSocket < 0 )
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
			close();
			throw Bu::TcpSocketException("Couldn't resolve hostname %s (%s).\n",
				sAddr.getStr(), bu_gai_strerror(ret));
		}

		bu_connect(
			nTcpSocket,
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
		FD_SET(nTcpSocket, &rfds);
		FD_ZERO(&wfds);
		FD_SET(nTcpSocket, &wfds);
		FD_ZERO(&efds);
		FD_SET(nTcpSocket, &efds);

		struct timeval tv;
		tv.tv_sec = nTimeout;
		tv.tv_usec = 0;
		
		retval = bu_select( nTcpSocket+1, &rfds, &wfds, &efds, &tv );

		if( retval == 0 )
		{
			close();
			throw ExceptionBase("Connection timeout.\n");
		}
		read( NULL, 0 ); // See if we can get any errors out of the way early.
	}

	if( bBlocking )
		setBlocking( bBlocking );
}

Bu::TcpSocket::~TcpSocket()
{
	close();
}

void Bu::TcpSocket::close()
{
	if( bActive )
	{
#ifndef WIN32
		fsync( nTcpSocket );
#endif
#ifdef WIN32
 #ifndef SHUT_RDWR
  #define SHUT_RDWR (SD_BOTH)
 #endif
#endif
		bu_shutdown( nTcpSocket, SHUT_RDWR );
		::close( nTcpSocket );
	}
	bActive = false;
}

Bu::size Bu::TcpSocket::read( void *pBuf, Bu::size nBytes )
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(nTcpSocket, &rfds);
	struct timeval tv = {0, 0};
	if( bu_select( nTcpSocket+1, &rfds, NULL, NULL, &tv ) < 0 )
	{
		int iErr = errno;
		close();
		throw TcpSocketException( TcpSocketException::cRead, strerror(iErr) );
	}
	if( FD_ISSET( nTcpSocket, &rfds ) || bBlocking )
	{
		int nRead = TEMP_FAILURE_RETRY( 
			bu_recv( nTcpSocket, (char *) pBuf, nBytes, 0 ) );
		if( nRead == 0 )
		{
			close();
			throw TcpSocketException( TcpSocketException::cClosed, "TcpSocket closed.");
		}
		if( nRead < 0 )
		{
#ifdef WIN32
			int iWSAError = bu_WSAGetLastError();
			if( iWSAError == WSAEWOULDBLOCK )
				return 0;
#else
			if( errno == ENETRESET || errno == ECONNRESET )
			{
				close();
				throw TcpSocketException( TcpSocketException::cClosed,
					strerror(errno) );
			}
			if( errno == EAGAIN )
				return 0;
			int iErr = errno;
			close();
			throw TcpSocketException( TcpSocketException::cRead, strerror(iErr) );
#endif
		}
		return nRead;
	}
	return 0;
}

Bu::size Bu::TcpSocket::read( void *pBuf, Bu::size nBytes,
		uint32_t nSec, uint32_t nUSec )
{
	struct timeval tv;
	Bu::size nRead = 0;
	
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(nTcpSocket, &rfds);

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
		bu_select( nTcpSocket+1, &rfds, NULL, NULL, &tv );
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

Bu::size Bu::TcpSocket::write( const void *pBuf, Bu::size nBytes )
{
//#ifdef WIN32
	int nWrote = TEMP_FAILURE_RETRY( 
		bu_send( nTcpSocket, (const char *) pBuf, nBytes, 0 ) );
//#else
//	int nWrote = TEMP_FAILURE_RETRY( ::write( nTcpSocket, pBuf, nBytes ) );
//#endif
	if( nWrote < 0 )
	{
#ifdef WIN32
		int iWSAError = bu_WSAGetLastError();
		if( iWSAError == WSAEWOULDBLOCK )
			return 0;
#else
		if( errno == EAGAIN ) return 0;
#endif
		throw TcpSocketException( TcpSocketException::cWrite, strerror(errno) );
	}
	return nWrote;
}

Bu::size Bu::TcpSocket::write( const void *pBuf, Bu::size nBytes, uint32_t nSec, uint32_t nUSec )
{
	struct timeval tv;
	Bu::size nWrote = 0;
	
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(nTcpSocket, &wfds);

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
		bu_select( nTcpSocket+1, NULL, &wfds, NULL, &tv );
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

Bu::size Bu::TcpSocket::tell()
{
	throw UnsupportedException();
}

void Bu::TcpSocket::seek( Bu::size )
{
	throw UnsupportedException();
}

void Bu::TcpSocket::setPos( Bu::size )
{
	throw UnsupportedException();
}

void Bu::TcpSocket::setPosEnd( Bu::size )
{
	throw UnsupportedException();
}

bool Bu::TcpSocket::isEos()
{
	return !bActive;
}

bool Bu::TcpSocket::canRead()
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(nTcpSocket, &rfds);
	struct timeval tv = { 0, 0 };
	int retval = bu_select( nTcpSocket+1, &rfds, NULL, NULL, &tv );
	if( retval == -1 )
		throw TcpSocketException(
			TcpSocketException::cBadRead,
			"Bad Read error"
			);

	if( !FD_ISSET( nTcpSocket, &rfds ) )
		return false;
	return true;
}

bool Bu::TcpSocket::canWrite()
{
	fd_set wfds;
	FD_ZERO(&wfds);
	FD_SET(nTcpSocket, &wfds);
	struct timeval tv = { 0, 0 };
	int retval = bu_select( nTcpSocket+1, NULL, &wfds, NULL, &tv );
	if( retval == -1 )
		throw TcpSocketException(
			TcpSocketException::cBadRead,
			"Bad Read error"
			);
	if( !FD_ISSET( nTcpSocket, &wfds ) )
		return false;
	return true;
}

bool Bu::TcpSocket::isReadable()
{
	return true;
}

bool Bu::TcpSocket::isWritable()
{
	return true;
}

bool Bu::TcpSocket::isSeekable()
{
	return false;
}

bool Bu::TcpSocket::isBlocking()
{
#ifndef WIN32
	return ((fcntl( nTcpSocket, F_GETFL, 0 ) & O_NONBLOCK) != O_NONBLOCK);
#else
	return false;
#endif
}

void Bu::TcpSocket::setBlocking( bool bBlocking )
{
	this->bBlocking = bBlocking;
#ifndef WIN32
	if( bBlocking )
	{
		fcntl( nTcpSocket, F_SETFL, fcntl( nTcpSocket, F_GETFL, 0 ) & (~O_NONBLOCK) );
	}
	else
	{
		fcntl( nTcpSocket, F_SETFL, fcntl( nTcpSocket, F_GETFL, 0 ) | O_NONBLOCK );
	}
#else
	u_Bu::size iMode;
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
	bu_ioctlsocket(nTcpSocket, FIONBIO, &iMode);
#endif	
}

void Bu::TcpSocket::setSize( Bu::size )
{
}

void Bu::TcpSocket::flush()
{
}

bool Bu::TcpSocket::isOpen()
{
	return bActive;
}

void Bu::TcpSocket::setAddress()
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	addr.sin_family = AF_INET;
	bu_getpeername( nTcpSocket, (sockaddr *)(&addr), &len );
	sAddress = bu_inet_ntoa( addr.sin_addr );
}

Bu::String Bu::TcpSocket::getAddress() const
{
	return sAddress;
}

Bu::TcpSocket::operator int() const
{
	return nTcpSocket;
}

Bu::size Bu::TcpSocket::getSize() const
{
	throw UnsupportedException();
}

Bu::size Bu::TcpSocket::getBlockSize() const
{
	return 1500; //TODO: Fix this, it's stupid.
}

Bu::String Bu::TcpSocket::getLocation() const
{
	return getAddress();
}


#include "bu/udpsocket.h"

#include "bu/sio.h"
using namespace Bu;
#include <fcntl.h>

#include <errno.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/utsname.h>

namespace Bu { subExceptionDef( UdpSocketException ) }

#define saTarget ( *((struct sockaddr_in *)paTarget) )

Bu::UdpSocket::UdpSocket( int iUdpSocket ) :
	iUdpSocket( iUdpSocket ),
	paTarget( NULL ),
	bBound( false )
{
}

Bu::UdpSocket::UdpSocket( const Bu::FString &sAddr, int iPort, int iFlags ) :
	iUdpSocket( 0 ),
	paTarget( NULL ),
	bBound( false )
{
	iUdpSocket = socket( PF_INET, SOCK_DGRAM, 0 );
	if( iUdpSocket < 0 )
	{
		throw UdpSocketException("Couldn't open udp socket: %s",
			strerror( errno )
			);
	}

	if( (iFlags&Broadcast) )
	{
		int broadcast = 1;
		if( (setsockopt( iUdpSocket, SOL_SOCKET, SO_BROADCAST,
			&broadcast, sizeof(broadcast) )) == -1)
		{
			throw UdpSocketException("Couldn't set udp socket to broadcast: %s",
				strerror( errno )
				);
		}
	}
	
	paTarget = new struct sockaddr_in;
	saTarget.sin_family = AF_INET;
	saTarget.sin_port = htons( iPort );
	saTarget.sin_addr.s_addr = inet_addr( sAddr.getStr() ); // INADDR_ANY;
	memset( saTarget.sin_zero, '\0', sizeof(saTarget.sin_zero) );
	
	if( (iFlags&Read) )
	{
		if( bind( iUdpSocket, (struct sockaddr*)paTarget, sizeof(struct sockaddr_in) )
				== -1 )
		{
			throw UdpSocketException("Couldn't bind port to udp socket: %s",
				strerror( errno )
				);
		}
		bBound = true;
	}
}

Bu::UdpSocket::~UdpSocket()
{
	close();
	delete (struct sockaddr_in *)paTarget;
	paTarget = NULL;
}

Bu::FString Bu::UdpSocket::addrToStr( const addr &a )
{
	Bu::FString sOut;
	sOut.format("%d.%d.%d.%d",
		(a&0xff),
		(a&0xff00)>>8,
		(a&0xff0000)>>16,
		(a&0xff000000)>>24
		);

	return sOut;
}

void Bu::UdpSocket::close()
{
	::close( iUdpSocket );
}

size_t Bu::UdpSocket::read( void *pBuf, size_t nBytes )
{
	return recv( iUdpSocket, pBuf, nBytes, 0 );
}

size_t Bu::UdpSocket::read( void *pBuf, size_t nBytes,
		Bu::UdpSocket::addr &aHost, int &iPort )
{
	sockaddr_in name;
	size_t size = sizeof(name);
	size_t ret = recvfrom( iUdpSocket, pBuf, nBytes, 0,
			(struct sockaddr *)&name, &size );
	aHost = name.sin_addr.s_addr;
	iPort = ntohs(name.sin_port);
	return ret;
}

size_t Bu::UdpSocket::write( const void *pBuf, size_t nBytes )
{
	if( bBound )
	{
		return sendto( iUdpSocket, pBuf, nBytes, 0, NULL, 0 );
	}
	else
	{
		return sendto( iUdpSocket, pBuf, nBytes, 0,
			(struct sockaddr*)paTarget, sizeof(struct sockaddr_in) );
	}
}

long Bu::UdpSocket::tell()
{
	throw Bu::UnsupportedException();
}

void Bu::UdpSocket::seek( long )
{
	throw Bu::UnsupportedException();
}

void Bu::UdpSocket::setPos( long )
{
	throw Bu::UnsupportedException();
}

void Bu::UdpSocket::setPosEnd( long )
{
	throw Bu::UnsupportedException();
}

bool Bu::UdpSocket::isEos()
{
	return false;
}

bool Bu::UdpSocket::isOpen()
{
	return true;
}

void Bu::UdpSocket::flush()
{
}

bool Bu::UdpSocket::canRead()
{
	return bBound;
}

bool Bu::UdpSocket::canWrite()
{
	return true;
}

bool Bu::UdpSocket::isReadable()
{
	return bBound;
}

bool Bu::UdpSocket::isWritable()
{
	return true;
}

bool Bu::UdpSocket::isSeekable()
{
	return false;
}

bool Bu::UdpSocket::isBlocking()
{
	return true;
}

void Bu::UdpSocket::setBlocking( bool bBlocking )
{
#ifndef WIN32
	if( bBlocking )
	{
		fcntl( iUdpSocket, F_SETFL, fcntl( iUdpSocket, F_GETFL, 0 ) & (~O_NONBLOCK) );
	}
	else
	{
		fcntl( iUdpSocket, F_SETFL, fcntl( iUdpSocket, F_GETFL, 0 ) | O_NONBLOCK );
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
	bu_ioctlsocket(iUdpSocket, FIONBIO, &iMode);
#endif	
}

void Bu::UdpSocket::setSize( long )
{
	throw Bu::UnsupportedException();
}


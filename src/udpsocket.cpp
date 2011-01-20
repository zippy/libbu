#include "bu/udpsocket.h"

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
	delete (struct sockaddr_in *)paTarget;
	paTarget = NULL;
}

void Bu::UdpSocket::close()
{
	::close( iUdpSocket );
}

size_t Bu::UdpSocket::read( void *pBuf, size_t nBytes )
{
	return recvfrom( iUdpSocket, pBuf, nBytes, 0, NULL, 0 );
}

size_t Bu::UdpSocket::read( void *pBuf, size_t nBytes,
		uint32_t nSec, uint32_t nUSec )
{
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

size_t Bu::UdpSocket::write( const void *pBuf, size_t nBytes,
		uint32_t nSec, uint32_t nUSec )
{
}

long Bu::UdpSocket::tell()
{
}

void Bu::UdpSocket::seek( long offset )
{
}

void Bu::UdpSocket::setPos( long pos )
{
}

void Bu::UdpSocket::setPosEnd( long pos )
{
}

bool Bu::UdpSocket::isEos()
{
}

bool Bu::UdpSocket::isOpen()
{
}

void Bu::UdpSocket::flush()
{
}

bool Bu::UdpSocket::canRead()
{
}

bool Bu::UdpSocket::canWrite()
{
}

bool Bu::UdpSocket::isReadable()
{
}

bool Bu::UdpSocket::isWritable()
{
}

bool Bu::UdpSocket::isSeekable()
{
}

bool Bu::UdpSocket::isBlocking()
{
}

void Bu::UdpSocket::setBlocking( bool bBlocking )
{
}

void Bu::UdpSocket::setSize( long iSize )
{
}


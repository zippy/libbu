#include "bu/udpsocket.h"

#include <errno.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/utsname.h>

namespace Bu { subExceptionDef( UdpSocketException ) }

Bu::UdpSocket::UdpSocket( int iUdpSocket ) :
	iUdpSocket( iUdpSocket )
{
}

Bu::UdpSocket::UdpSocket( const Bu::FString &sAddr, int iPort,
		bool bBroadcast ) :
	iUdpSocket( 0 )
{
	struct sockaddr_in name;

	iUdpSocket = socket( PF_INET, SOCK_DGRAM, 0 );
	if( iUdpSocket < 0 )
	{
		throw UdpSocketException("Couldn't open udp socket: %s",
			strerror( errno )
			);
	}

	if( bBroadcast )
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

	name.sin_family = AF_INET;
	name.sin_port = htons( iPort );
	name.sin_addr.s_addr = INADDR_ANY;
	memset( name.sin_zero, '\0', sizeof(name.sin_zero) );
	
	if( bind( iUdpSocket, (struct sockaddr*) &name, sizeof(name) ) == -1 )
	{
		throw UdpSocketException("Couldn't bind port to udp socket: %s",
			strerror( errno )
			);
	}
	
	name.sin_family = AF_INET;
	name.sin_port = htons( iPort );
	name.sin_addr.s_addr = inet_addr( sAddr.getStr() );
	memset( name.sin_zero, '\0', sizeof(name.sin_zero) );
/*
	while( true )
	{
		nbytes = sendto( iUdpSocket, "12345", 5, 0,
			(struct sockaddr *)&name, size );
		if( nbytes < 0 )
		{
			perror("sendto");
//			exit( 0 );
		}

		printf("Client wrote something\n");
		int nQueen = sockServe.accept( 3, 0 );
		if( nQueen >= 0 )
		{
			close( iUdpSocket );
			return nQueen;
		}
	}
	*/
}

void Bu::UdpSocket::close()
{
	::close( iUdpSocket );
}

size_t Bu::UdpSocket::read( void *pBuf, size_t nBytes )
{
}

size_t Bu::UdpSocket::read( void *pBuf, size_t nBytes,
		uint32_t nSec, uint32_t nUSec=0 )
{
}

size_t Bu::UdpSocket::write( const void *pBuf, size_t nBytes )
{
	return sendto( iUdpSocket, pBuf, nBytes, 0,
		(struct sockaddr *)&name, size );
}

size_t Bu::UdpSocket::write( const void *pBuf, size_t nBytes,
		uint32_t nSec, uint32_t nUSec=0 )
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

void Bu::UdpSocket::setBlocking( bool bBlocking=true )
{
}

void Bu::UdpSocket::setSize( long iSize )
{
}


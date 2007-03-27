#include "connection.h"
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
#include "exceptions.h"

// Read buffer size...maybe fix wierd issues...
#define RBS		(1024*10)

Connection::Connection()
{
	nSocket = -1;
	bActive = false;
	bDisconnectMe = false;
	pProtocol = NULL;
}

Connection::~Connection()
{
	if( pProtocol != NULL ) delete pProtocol;
}

void Connection::ensureCapacity( int nSize )
{
	xOutputBuf.ensureCapacity( nSize );
}

bool Connection::appendOutput( const char *lpOutput, int nSize )
{
	return xOutputBuf.appendData( lpOutput, nSize );
}

bool Connection::appendOutput( const char lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const short lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const int lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const long lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const float lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const double lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const unsigned char lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const unsigned short lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const unsigned long lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendOutput( const unsigned int lOutput )
{
	return xOutputBuf.appendData( lOutput );
}

bool Connection::appendInput( const char *lpInput, int nSize )
{
	return xInputBuf.appendData( lpInput, nSize );
}

int Connection::scanInputFor( char cTarget )
{
	const char *lpTmp = xInputBuf.getData();
	int jMax = xInputBuf.getLength();

	for( int j = 0; j < jMax; j++ )
	{
		if( lpTmp[j] == cTarget )
		{
			return j;
		}
	}

	return -1;
}

const char *Connection::getOutput()
{
	return xOutputBuf.getData();
}

const char *Connection::getInput()
{
	return xInputBuf.getData();
}

void Connection::setSocket( int nNewSocket )
{
	nSocket = nNewSocket;
}

int Connection::getSocket()
{
	return nSocket;
}

bool Connection::isActive()
{
	return bActive;
}

void Connection::close()
{
	if( bActive )
	{
		fsync( nSocket );
		::close( nSocket );
	}
	bActive = false;
	//nSocket = -1;
	xInputBuf.clearData();
	xOutputBuf.clearData();
	if( pProtocol != NULL )
	{
		delete pProtocol;
		pProtocol = NULL;
	}
}

bool Connection::open( int nNewSocket )
{
	bActive = true;
	setSocket( nNewSocket );
	bDisconnectMe = false;

	return true;
}

bool Connection::open( const char *sAddr, int nPort )
{
	struct sockaddr_in xServerName;
	bActive = false;
     
	/* Create the socket. */
	nSocket = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( nSocket < 0 )
	{
		bActive = false;
		return false;
	}
     
	/* Connect to the server. */
	{
		struct hostent *hostinfo;
     
		xServerName.sin_family = AF_INET;
		xServerName.sin_port = htons( nPort );
		hostinfo = gethostbyname( sAddr );
		if (hostinfo == NULL)
		{
			return false;
		}
		xServerName.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	}

	int ret = connect(
		nSocket,
		(struct sockaddr *)&xServerName,
		sizeof(xServerName)
		);

	if( ret < 0 )
	{
		return false;
	}

	bActive = true;
	bDisconnectMe = false;

	return true;
}

int Connection::readInput()
{
	char buffer[RBS];
	int nbytes;
	int nTotalRead=0;

	for(;;)
	{
		//memset( buffer, 0, RBS );

		nbytes = read( nSocket, buffer, RBS );
		if( nbytes < 0 && errno != 0 && errno != EAGAIN )
		{
			printf("errno: %d, %s\n", errno, strerror( errno ) );
			/* Read error. */
			//perror("readInput");
			throw ConnectionException( excodeReadError, "Read error: %s", strerror( errno ) );
		}
		else
		{
			if( nbytes <= 0 )
				break;
			nTotalRead += nbytes;
			appendInput( buffer, nbytes );
			/* Data read. */
			if( nbytes < RBS )
			{
				break;
			}

			/* New test, if data is divisible by RBS bytes on some libs the
			 * read could block, this keeps it from happening.
			 */
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

	if( pProtocol != NULL && nTotalRead > 0 )
	{
		pProtocol->onNewData();
	}

	return nTotalRead;
}

bool Connection::readInput( int nSec, int nUSec, int *pnSecBack, int *pnUSecBack )
{
	fd_set rfds, efds;
	struct timeval tv, start, end;
	struct timezone tz;
	int retval;
	
	gettimeofday( &start, &tz );

	FD_ZERO(&rfds);
	FD_SET(nSocket, &rfds);
	FD_ZERO(&efds);
	FD_SET(nSocket, &efds);

	tv.tv_sec = nSec;
	tv.tv_usec = nUSec;

	//printf("Starting at %d %d\n", nSec, nUSec );
	retval = select( nSocket+1, &rfds, NULL, NULL, &tv );

	if( retval == -1 )
	{
		// Oh my god!!! some kind of horrible problem!!!!
		throw ConnectionException( excodeBadReadError, "Bad Read error");
		return false;
	}
	else if( retval )
	{
		//printf("retval=%d, nSocket=%d,%d, sec=%d, usec=%d\n", retval, nSocket, FD_ISSET( nSocket, &rfds ), tv.tv_sec, tv.tv_usec );
		// None of them have data, but the connection is still active.
		if( FD_ISSET( nSocket, &rfds ) )
		{
			if( readInput() == 0 )
			{
				throw ConnectionException( excodeConnectionClosed, "Connection closed");	}
		}
	}

	gettimeofday( &end, &tz );

	int st, ust;
	st = nSec - ( end.tv_sec - start.tv_sec );
	if( ( end.tv_usec - start.tv_usec ) > nUSec )
	{
		(st)--;
		ust = 1000000 - (end.tv_usec - start.tv_usec);
	}
	else
	{
		ust = nUSec - (end.tv_usec - start.tv_usec);
	}

	if( st < 0 )
	{
		st = ust = 0;
	}

	if( pnSecBack )
	{
		*pnSecBack = st;
		*pnUSecBack = ust;
	}

	//printf("New time:  %d %d\n", *pnSecBack, *pnUSecBack );

	return true;
}

void Connection::waitForInput( int nBytesIn, int nSec, int nUSec )
{
	int rlen = getInputAmnt();

	if( rlen >= nBytesIn )
		return;

	while( rlen < nBytesIn )
	{
		if( nSec == 0 && nUSec == 0 )
		{
			throw ConnectionException( excodeSocketTimeout, "Timed out while waiting for %d bytes.", nBytesIn );
		}
		readInput( nSec, nUSec, &nSec, &nUSec );
		rlen = getInputAmnt();
	}
}

bool Connection::clearOutput()
{
	return xOutputBuf.clearData();
}

bool Connection::clearInput()
{
	return xInputBuf.clearData();
}

#define min( a, b )   ((a<b)?(a):(b))

bool Connection::writeOutput()
{
	//int nBytes = TEMP_FAILURE_RETRY( write( nSocket, xOutputBuf.getData(), min( RBS, xOutputBuf.getLength() ) ) );
	int nBytes = TEMP_FAILURE_RETRY( write( nSocket, xOutputBuf.getData(), xOutputBuf.getLength() ) );
	if( nBytes < 0 )
	{
		perror("writeOutput");
		return true;
	}
	/*	
	if( nBytes < xOutputBuf.getLength() )
	{
		printf("Havn't written all the data (%d/%d/%d%%)\n", nBytes, xOutputBuf.getLength(), nBytes/(xOutputBuf.getLength()*100) );
	}
	else
	{
		printf("Wrote all pending data.\n");
	}
	*/
	xOutputBuf.usedData( nBytes );
	//clearOutput();

	return true;
}

bool Connection::writeAllOutput()
{
	while( hasOutput() ) writeOutput();
	return true;
}

bool Connection::hasOutput()
{
	if( xOutputBuf.getLength() == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Connection::hasInput()
{
	if( xInputBuf.getLength() == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Connection::usedInput( int nAmount )
{
	return xInputBuf.usedData( nAmount );
}

bool Connection::needDisconnect()
{
	return bDisconnectMe;
}

void Connection::disconnect()
{
	bDisconnectMe = true;
}

void Connection::setProtocol( class Protocol *pNewProtocol )
{
	pProtocol = pNewProtocol;
	pProtocol->setConnection( this );
}

int Connection::getInputAmnt()
{
	return xInputBuf.getLength();
}

int Connection::getOutputAmnt()
{
	return xOutputBuf.getLength();
}

class Protocol *Connection::getProtocol()
{
	return pProtocol;
}

void Connection::printInputDebug( const char *lpPrefix, FILE *fh, int nBytesMax )
{
	printDataDebug(
		(const unsigned char *)xInputBuf.getData(),
		xInputBuf.getLength(),
		"input",
		lpPrefix,
		fh,
		nBytesMax
		);
}

void Connection::printOutputDebug( const char *lpPrefix, FILE *fh, int nBytesMax )
{
	printDataDebug(
		(const unsigned char *)xOutputBuf.getData(),
		xOutputBuf.getLength(),
		"output",
		lpPrefix,
		fh,
		nBytesMax
		);
}

void Connection::printDataDebug( const unsigned char *pData, long nDataLen, const char *lpName, const char *lpPrefix, FILE *fh, int nBytesMax )
{
	if( nBytesMax > 0 )
	{
		nDataLen = (nBytesMax<nDataLen)?(nBytesMax):(nDataLen);
	}

	fprintf( fh, "%sDisplaying %ld bytes of %s.\n", lpPrefix, nDataLen, lpName );
	int j = 0;
	fprintf( fh, lpPrefix );
	for( int l = 0; l < 8*3+2*8+2; l++ ) fprintf( fh, (l!=8*3)?("-"):("+") ); fprintf( fh, "\n");
	for(;;)
	{
		fprintf( fh, lpPrefix );
		int kmax = 8;
		if( nDataLen-j < 8 ) kmax = nDataLen-j;
		for(int k = 0; k < 8; k++ )
		{
			if( k < kmax )
			{
				fprintf( fh, "%02X ", (int)((unsigned char)pData[j+k]) );
			}
			else
			{
				fprintf( fh, "-- ");
			}
		}
		printf("| ");
		for(int k = 0; k < kmax; k++ )
		{
			fprintf( fh, "%c ", (pData[j+k]>32 && pData[j+k]<=128)?(pData[j+k]):('.') );
		}
		fprintf( fh, "\n");
		j += kmax;
		if( j >= nDataLen ) break;
	}
	fprintf( fh, lpPrefix );
	for( int l = 0; l < 8*3+2*8+2; l++ ) fprintf( fh, (l!=8*3)?("-"):("+") ); fprintf( fh, "\n");
}


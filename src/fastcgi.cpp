#include "bu/fastcgi.h"

#include <arpa/inet.h>
#include <errno.h>

#include "bu/membuf.h"

#include "bu/sio.h"
using Bu::sio;
using Bu::Fmt;

Bu::FastCgi::FastCgi() :
	pSrv( NULL ),
	bRunning( true )
{
	pSrv = new Bu::ServerSocket( STDIN_FILENO, false );
}

Bu::FastCgi::FastCgi( int iPort ) :
	pSrv( NULL ),
	bRunning( true )
{
	pSrv = new Bu::ServerSocket( iPort );
}

Bu::FastCgi::~FastCgi()
{
}

bool Bu::FastCgi::isEmbedded()
{
	struct sockaddr name;
	socklen_t namelen = sizeof(name);
	if( getpeername( STDIN_FILENO, &name, &namelen ) != 0 &&
		errno == ENOTCONN )
	{
		sio << "errno = " << errno << " (" << strerror( errno ) << ")" <<
			sio.nl;
		sio << "Socket found" << sio.nl;
		return true;
	}
	else
	{
		sio << "errno = " << errno << " (" << strerror( errno ) << ")" <<
			sio.nl;
		sio << "No socket detected, running in standalone mode" << sio.nl;
		return false;
	}
}

void Bu::FastCgi::read( Bu::Socket &s, Bu::FastCgi::Record &r )
{
	s.read( &r, sizeof(Record) );
	r.uRequestId = ntohs( r.uRequestId );
	r.uContentLength = ntohs( r.uContentLength );
}

void Bu::FastCgi::write( Bu::Socket &s, Bu::FastCgi::Record r )
{
	r.uRequestId = htons( r.uRequestId );
	r.uContentLength = htons( r.uContentLength );
	s.write( &r, sizeof(Record) );
}

void Bu::FastCgi::read( Bu::Socket &s, Bu::FastCgi::BeginRequestBody &b )
{
	s.read( &b, sizeof(BeginRequestBody) );
	b.uRole = ntohs( b.uRole );
}

void Bu::FastCgi::write( Bu::Socket &s, Bu::FastCgi::EndRequestBody b )
{
	b.uStatus = htonl( b.uStatus );
	s.write( &b, sizeof(b) );
}

uint32_t Bu::FastCgi::readLen( Bu::Socket &s, uint16_t &uRead )
{
	uint8_t uByte[4];
	s.read( uByte, 1 );
	uRead++;
	if( uByte[0] >> 7 == 0 )
		return uByte[0];
	
	s.read( uByte+1, 3 );
	uRead += 3;
	return ((uByte[0]&0x7f)<<24)|(uByte[1]<<16)|(uByte[2]<<8)|(uByte[3]);
}

void Bu::FastCgi::readPair( Bu::Socket &s, StrHash &hParams, uint16_t &uRead )
{
	uint32_t uName = readLen( s, uRead );
	uint32_t uValue = readLen( s, uRead );
	uRead += uName + uValue;
	unsigned char *sName = new unsigned char[uName];
	s.read( sName, uName );
	Bu::FString fsName( (char *)sName, uName );
	delete[] sName;

	if( uValue > 0 )
	{
		unsigned char *sValue = new unsigned char[uValue];
		s.read( sValue, uValue );
		Bu::FString fsValue( (char *)sValue, uValue );
		hParams.insert( fsName, fsValue );
		delete[] sValue;
	}
	else
	{
		hParams.insert( fsName, "" );
	}
}

void Bu::FastCgi::run()
{
	bRunning = true;
	while( bRunning )
	{
		int iSock = pSrv->accept( 5 );
		if( iSock < 0 )
			continue;

		Bu::Socket s( iSock );
		s.setBlocking( true );
		try
		{
			for(;;)
			{
				Record r;
				read( s, r );
				while( aChannel.getSize() < r.uRequestId )
					aChannel.append( NULL );
				Channel *pChan = NULL;
				if( r.uRequestId > 0 )
					pChan = aChannel[r.uRequestId-1];

				sio << "Record (id=" << r.uRequestId << ", len=" << 
					r.uContentLength << ", pad=" << 
					(int)r.uPaddingLength << "):  ";
				fflush( stdout );
				
				switch( (RequestType)r.uType )
				{
					case typeBeginRequest:
						sio << "Begin Request.";
						{
							BeginRequestBody b;
							read( s, b );
							if( pChan != NULL )
							{
								sio << "Error!!!" << sio.nl;
								return;
							}
							pChan = aChannel[r.uRequestId-1] = new Channel();
						}
						break;

					case typeParams:
						sio << "Params.";
						if( r.uContentLength == 0 )
						{
							pChan->uFlags |= chflgParamsDone;
						}
						else
						{
							uint16_t uUsed = 0;
							while( uUsed < r.uContentLength )
							{
								readPair( s, pChan->hParams, uUsed );
							}
						}
						break;

					case typeStdIn:
						sio << "StdIn.";
						if( r.uContentLength == 0 )
						{
							pChan->uFlags |= chflgStdInDone;
						}
						else
						{
							char *buf = new char[r.uContentLength];
							sio << " (read " << s.read( buf, r.uContentLength )
								<< ")";
							pChan->sStdIn.append( buf, r.uContentLength );
							delete[] buf;
						}
						break;

					case typeData:
						sio << "Data.";
						if( r.uContentLength == 0 )
						{
							pChan->uFlags |= chflgDataDone;
						}
						else
						{
							char *buf = new char[r.uContentLength];
							s.read( buf, r.uContentLength );
							pChan->sData.append( buf, r.uContentLength );
							delete[] buf;
						}
						break;

					case typeStdOut:
					case typeStdErr:
					case typeEndRequest:
					case typeAbortRequest:
					case typeGetValuesResult:
						sio << "Scary.";
						// ??? we shouldn't get these.
						break;
					
				}

				sio << sio.nl;

				if( pChan )
				{
					if( pChan->uFlags == chflgAllDone )
					{
						Bu::MemBuf mStdOut, mStdErr;
						int iRet = request(
							pChan->hParams, pChan->sStdIn,
							mStdOut, mStdErr
							);

						Bu::FString &sStdOut = mStdOut.getString();
						Bu::FString &sStdErr = mStdErr.getString();

						Record rOut;
						rOut.uVersion = 1;
						rOut.uRequestId = r.uRequestId;
						rOut.uPaddingLength = 0;
						if( sStdOut.getSize() > 0 )
						{
							rOut.uType = typeStdOut;
							rOut.uContentLength = sStdOut.getSize();
							write( s, rOut );
							s.write( sStdOut );
						}
						rOut.uType = typeStdOut;
						rOut.uContentLength = 0;
						write( s, rOut );
						if( sStdErr.getSize() > 0 )
						{
							rOut.uType = typeStdErr;
							rOut.uContentLength = sStdErr.getSize();
							write( s, rOut );
							s.write( sStdOut );
						}
						rOut.uType = typeStdErr;
						rOut.uContentLength = 0;
						write( s, rOut );

						rOut.uType = typeEndRequest;
						rOut.uContentLength = 8;
						write( s, rOut );
						
						EndRequestBody b;
						memset( &b, 0, sizeof(b) );
						b.uStatus =  iRet;
						write( s, b );

						delete pChan;
						aChannel[r.uRequestId-1] = NULL;
					}
				}
			}
		}
		catch( Bu::SocketException &e )
		{
			//sio << "Bu::SocketException: " << e.what() << sio.nl;
		}
	}
}


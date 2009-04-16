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
	int iRead = s.read( &r, sizeof(Record) );
	if( iRead != sizeof(Record) )
		throw Bu::SocketException("Hey, the size %d is wrong for Record. (%s)",
			iRead, strerror( errno ) );
	r.uRequestId = ntohs( r.uRequestId );
	r.uContentLength = ntohs( r.uContentLength );
}

void Bu::FastCgi::write( Bu::Socket &s, Bu::FastCgi::Record r )
{
	sio << "Out -> " << r << sio.nl;
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

bool Bu::FastCgi::hasChannel( int iChan )
{
	if( aChannel.getSize() < iChan )
		return false;
	if( aChannel[iChan-1] == NULL )
		return false;
	return true;
}

Bu::Formatter &Bu::operator<<( Bu::Formatter &f, const Bu::FastCgi::Record &r )
{
	f << "[Ver=" << (uint32_t)r.uVersion <<
		", Type=" << (uint32_t)r.uType <<
		", Req=" << (uint32_t)r.uRequestId <<
		", clen=" << (uint32_t)r.uContentLength <<
		", plen=" << (uint32_t)r.uPaddingLength <<
		", resv=" << (uint32_t)r.uReserved <<
		"]";
	return f;
}

void Bu::FastCgi::run()
{
	sio << "sizeof(Bu::FastCgi::Record) = " << sizeof(Record) << sio.nl;
	bRunning = true;
	while( bRunning )
	{
		int iSock = pSrv->accept( 5 );
		if( iSock < 0 )
			continue;

		Bu::Socket s( iSock );
		s.setBlocking( true );
		sio << "Got connection, blocking?  " << s.isBlocking() << sio.nl;
		try
		{
			for(;;)
			{
				Record r;
				memset( &r, 0, sizeof(r) );
//				try
//				{
					read( s, r );
//				}
//				catch( Bu::ExceptionBase &e )
//				{
//					sio << "Error: " << e.what() << ", " << s.isOpen() <<
//						sio.nl;
//					continue;
//				}
				Channel *pChan = NULL;
				if( r.uRequestId > 0 )
				{
					if( !hasChannel( r.uRequestId ) &&
						r.uType != typeBeginRequest )
					{
						sio << "Error, stream data without stream." << sio.nl;
						sio << r << sio.nl;
						if( r.uContentLength > 0 )
						{
							char *buf = new char[r.uContentLength];
							sio << " (read " << s.read( buf, r.uContentLength )
								<< "/" << r.uContentLength << "):" << sio.nl;
							sio.write( buf, r.uContentLength );
							sio << sio.nl << sio.nl;
						}
					}
					while( aChannel.getSize() < r.uRequestId )
						aChannel.append( NULL );
					if( r.uRequestId > 0 )
						pChan = aChannel[r.uRequestId-1];
				}

				sio << "Record (id=" << r.uRequestId << ", len=" << 
					r.uContentLength << ", pad=" << 
					(unsigned int)r.uPaddingLength << "):  ";
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
							int iTotal = 0;
							do
							{
								size_t iRead = s.read(
									buf, r.uContentLength-iTotal );
								iTotal += iRead;
								sio << " (read " << iRead << " " << iTotal
									<< "/" << r.uContentLength << ")";
								pChan->sStdIn.append( buf, iRead );
							} while( iTotal < r.uContentLength );
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
						sio << "All done, generating output." << sio.nl;
						Bu::MemBuf mStdOut, mStdErr;
						int iRet = request(
							pChan->hParams, pChan->sStdIn,
							mStdOut, mStdErr
							);

						Bu::FString &sStdOut = mStdOut.getString();
						Bu::FString &sStdErr = mStdErr.getString();

						Record rOut;
						memset( &rOut, 0, sizeof(rOut) );
						rOut.uVersion = 1;
						rOut.uRequestId = r.uRequestId;
						rOut.uPaddingLength = 0;
						rOut.uType = typeStdOut;
						if( sStdOut.getSize() > 0 )
						{
							for( int iPos = 0; iPos < sStdOut.getSize();
								 iPos += 65528 )
							{
								int iSize = sStdOut.getSize()-iPos;
								if( iSize > 65528 )
									iSize = 65528;
								rOut.uContentLength = iSize;
								write( s, rOut );
								int iAmnt = s.write(
									sStdOut.getStr()+iPos, iSize );
								sio << "Wrote " << iAmnt <<
									" of " << iSize << sio.nl;
							}
						}
						rOut.uContentLength = 0;
						write( s, rOut );

						rOut.uType = typeStdErr;
						if( sStdErr.getSize() > 0 )
						{
							for( int iPos = 0; iPos < sStdErr.getSize();
								 iPos += 65528 )
							{
								int iSize = sStdErr.getSize()-iPos;
								if( iSize > 65528 )
									iSize = 65528;
								rOut.uContentLength = iSize;
								write( s, rOut );
								int iAmnt = s.write(
									sStdErr.getStr()+iPos, iSize );
								sio << "Wrote " << iAmnt <<
									" of " << iSize << sio.nl;
							}
						}
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
			sio << "Bu::SocketException: " << e.what() << sio.nl <<
				"\tSocket open:  " << s.isOpen() << sio.nl;
		}
	}
}


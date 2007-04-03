#include "httpget.h"
#include "exceptions.h"
#include "connection.h"
#include <stdio.h>

char HttpGet::hexcode[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

HttpGet::HttpGet() :
	nPort( 80 ),
	sUserAgent("libbu++; HttpGet")
{
}

HttpGet::HttpGet( const std::string &url ) :
	nPort( 80 )
{
	setURL( url );
}

HttpGet::~HttpGet()
{
}

void HttpGet::setURL( const std::string &url )
{
	int len = url.size();
	//printf("Full URL: %s\n", url.c_str() );
	int pos = url.find("://");
	sProto.assign( url, 0, pos );
	//printf("Protocol: %s\n", sProto.c_str() );
	
	int pos2 = url.find("/", pos+3 );
	if( pos2 >= 0 )
	{
		sHost.assign( url, pos+3, pos2-pos-3 );
	}
	else
	{
		sHost.assign( url, pos+3, std::string::npos );
	}

	int pos3 = sHost.find(":");
	if( pos3 >= 0 )
	{
		nPort = strtol( sHost.c_str()+pos3+1, NULL, 10 );
		sHost.erase( pos3 );
	}
	//printf("Hostname: %s\n", sHost.c_str() );
	//printf("Port: %d\n", nPort );

	pos3 = url.find("?", pos2+1 );
	if( pos3 >= 0 )
	{
		sPath.assign( url, pos2, pos3-pos2 );
		//printf("Path: %s\n", sPath.c_str() );
		for(;;)
		{
			int end = pos3+1;
			for(; url[end] != '=' && url[end] != '&' && end < len; end++ );
			std::string sKey, sValue;
			sKey.assign( url, pos3+1, end-pos3-1 );
			if( url[end] == '=' )
			{
				pos3 = end;
				for( end++; url[end] != '&' && end < len; end++ );
				sValue.assign( url, pos3+1, end-pos3-1 );
				pos3 = end;
			}
			else
			{
			}
			lParams.push_back( StringPair( sKey, sValue ) );
			//printf("Param: %s = %s\n", sKey.c_str(), sValue.c_str() );
			if( end+1 >= len ) break;
		}
	}
	else
	{
		sPath.assign( url, pos2, std::string::npos );
		//printf("Path: %s\n", sPath.c_str() );
	}

	//printf("\n");
}

void HttpGet::addParam( const std::string &key, const std::string &value )
{
	lParams.push_back( StringPair( key, value ) );
}

std::string HttpGet::escape( const std::string &src )
{
	std::string escaped("");
	for( std::string::const_iterator i = src.begin(); i != src.end(); i++ )
	{
		unsigned char j = *i;
		if( (j >= '0' && j <= '9') || 
			(j >= 'a' && j <= 'z') ||
			(j >= 'A' && j <= 'Z') ||
			j == '$' ||
			j == '-' ||
			j == '_' ||
			j == '.' ||
			j == '+' ||
			j == '!' ||
			j == '*' ||
			j == '\'' ||
			j == '(' ||
			j == ')' )
		{
			escaped += j;
		}
		else
		{
			escaped += "%";
			escaped += hexcode[j>>4];
			escaped += hexcode[j&0x0F];
		}
	}

	return escaped;
}

SBuffer *HttpGet::get()
{
	std::string sData;
	sData = "GET " + sPath;
	if( !lParams.empty() )
	{
		sData += "?";
		for( std::list<StringPair>::iterator i = lParams.begin();
			 i != lParams.end(); i++ )
		{
			if( i != lParams.begin() )
				sData += "&";

			if( (*i).second == "" )
			{
				sData += escape( (*i).first );
			}
			else
			{
				sData += escape( (*i).first );
				sData += "=";
				sData += escape( (*i).second );
			}
		}
	}

	sData += " HTTP/1.1\r\n"
		"User-Agent: " + sUserAgent + "\r\n"
		"Connection: close\r\n"
		"Host: " + sHost + "\r\n"
		"Content-type: application/x-www-form-urlencoded\r\n\r\n";

	//printf("Connection content:\n\n%s\n\n", sData.c_str() );

	Connection con;
	//printf("Opening connection...\n");
	con.open( sHost.c_str(), nPort );
	{
		int nSocket = con.getSocket();
		fd_set rfds, wfds, efds;
		int retval;
		
		FD_ZERO(&rfds);
		FD_SET(nSocket, &rfds);
		FD_ZERO(&wfds);
		FD_SET(nSocket, &wfds);
		FD_ZERO(&efds);
		FD_SET(nSocket, &efds);

		struct timeval tv;
		tv.tv_sec = 4;
		tv.tv_usec = 0;
		
		//printf("Selecting on socket, can we read, write, etc?\n");
		retval = select( nSocket+1, &rfds, &wfds, &efds, &tv );
		/*printf("About to write: sock=%d, r=%d, w=%d, e=%d, ret=%d\n",
			nSocket,
			FD_ISSET( nSocket, &rfds ),
			FD_ISSET( nSocket, &wfds ),
			FD_ISSET( nSocket, &efds ),
			retval
			);*/

		if( retval == 0 )
		{
			//printf("Timeout on connection.\n");
			con.close();
			throw ExceptionBase("Connection Timeout on open.\n");
		}
		
	}
	con.appendOutput( sData.c_str(), sData.size() );
	//printf("Writing to socket...\n");
	con.writeOutput();
	//printf("Data written...\n");
	int nSec = 5;
	int nUSec = 0;
	int nLastAmnt = con.getInputAmnt();
	try
	{
		double dTotTime = 0.0;
		//printf("About to read input...\n");
		while( con.readInput( nSec, nUSec, &nSec, &nUSec ) )
		{
			if( nLastAmnt == con.getInputAmnt() )
			{
				if( nSec <= 0 && nUSec <= 0 )
				{
					//printf("out of time, closing up.\n");
					con.close();
					throw ExceptionBase("Connection Timeout.\n");
				}
				if( nSec == 5 && nUSec == 0 )
				{
					//printf("No new data, breaking.\n");
					break;
				}
			}
			else
			{
				dTotTime += (5.0-(nSec+nUSec/1000000.0));
				printf("\rRead %db at %.2fkb/sec",
					con.getInputAmnt(),
					((double)(con.getInputAmnt())/1024.0) / dTotTime
					);
				fflush( stdout );
				nSec = 5;
				nUSec = 0;
				nLastAmnt = con.getInputAmnt();
			}
		}
	}
	catch( ConnectionException &e )
	{
		//con.close();
		if( strcmp( e.what(), "Connection closed" ) )
			printf("\nConnectionException:  %s\n", e.what() );
	}

	int total = con.getInputAmnt();
	const char *dat = con.getInput();
	//printf("\n===> Final size %d\n", total );
	for( int i = 0; i < total; i++ )
	{
		if( !memcmp( dat+i, "\r\n\r\n", 4 ) )
		{
			SBuffer *buf = new SBuffer;
			buf->write( dat+i+4, total-i-4 );
			buf->setPos( 0 );
			con.close();
			return buf;
		}
	}
	con.close();

	//printf("\n\n%s\n\n", dat );

	throw ExceptionBase("Something went wrong, incomplete response?  fix this.\n");
}


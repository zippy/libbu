#include <dirent.h>
#include <sys/wait.h>
#include <errno.h>
#include "protocolhttp.h"

#define CRLF "\x0D\x0A"
#define CR '\x0D'
#define LF '\x0A'

using namespace Bu;

Bu::ProtocolHttp::ProtocolHttp()
{
}

Bu::ProtocolHttp::~ProtocolHttp()
{
}

void Bu::ProtocolHttp::onNewConnection( Bu::Client *pClient )
{
	this->pClient = pClient;

	iState = 0;
}

#define SDB( i ) printf("state %d: %d, \"%s\"\n", i, tt, sToken.getStr() )

void Bu::ProtocolHttp::onNewData( Bu::Client *pClient )
{
	for(;;)
	{
		Bu::FString sToken;
		TokenType tt = getToken( sToken );

		if( tt == ttOutOfData )
			return;

		switch( iState )
		{
			case 0:		// Initial header
				SDB( 0 );
				sMethod = sToken;
				iState = 1;
				break;

			case 1:		// Misc headers
				SDB( 1 );
				sPath = sToken;
				iState = 2;
				break;

			case 2:		// Content
				SDB( 2 );
				if( strncmp( sToken.getStr(), "HTTP/", 5 ) )
				{
					printf("not http, disconnect.\n");
					pClient->disconnect();
					return;
				}
				else
				{
					char *s, *s2;
					s = sToken.getStr()+5;
					iMajor = strtol( s, &s2, 10 );
					iMinor = strtol( s2+1, NULL, 10 );
					printf("HTTP: %d.%d\n", iMajor, iMinor );
					iState = 3;
				}
				break;

			case 3:
				SDB( 3 );
				pClient->disconnect();
				return;
				break;
		}
	}
}

Bu::ProtocolHttp::TokenType Bu::ProtocolHttp::getToken( Bu::FString &line )
{
	char s;
	int jmax = pClient->getInputSize();
	bool bNonWS = false;
	
	for( int j = 0; j < jmax; j++ )
	{
		pClient->peek( &s, 1, j );
		if( iState > 2 && isSeperator( s ) )
		{
			if( j == 0 )
			{
				line += s;
				pClient->seek( 1 );
				return ttSeperator;
			}
			else
			{
				pClient->seek( j );
				return ttString;
			}
		}
		else if( isWS( s ) )
		{
			if( bNonWS )
			{
				pClient->seek( j );
				return ttString;
			}
		}
		else if( s == CR )
		{
			if( pClient->getInputSize() < 3 )
				return ttOutOfData;
			
			char ss[2];
			pClient->peek( ss, 2, j+1 );
			if( ss[0] == LF && ss[1] != ' ' && ss[1] != '\t' )
			{
				if( bNonWS )
				{
					pClient->seek( j );
					return ttString;
				}
				else
				{
					pClient->seek( 2 );
					return ttNewline;
				}
			}
			
			j += 2;
			if( bNonWS )
			{
				pClient->seek( j );
				return ttString;
			}
		}
		else
		{
			line += s;
			bNonWS = true;
		}
	}

	return ttOutOfData;
}

bool Bu::ProtocolHttp::isWS( char buf )
{
	return (buf == ' ' || buf == '\t');
}

bool Bu::ProtocolHttp::isSeperator( char buf )
{
	return (buf == '(' || buf == ')' || buf == '<' || buf == '>' ||
			buf == '@' || buf == ',' || buf == ';' || buf == ':' ||
			buf == '\\' || buf == '\"' || buf == '/' || buf == '[' ||
			buf == ']' || buf == '?' || buf == '=' || buf == '{' ||
			buf == '}' );
}


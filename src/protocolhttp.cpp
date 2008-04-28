/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <dirent.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include "bu/protocolhttp.h"
#include "bu/logger.h"

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

#define SDB( i ) { }
//#define SDB( i ) printf("state %d: %d, \"%s\"\n", i, tt, sToken.getStr() )

void Bu::ProtocolHttp::onNewData( Bu::Client *pClient )
{
/*	logHexDump(
		1,
		pClient->getInput().getStr(),
		pClient->getInput().getSize(),
		"input"
		);*/

	for(;;)
	{
		Bu::FString sToken;
		TokenType tt = getToken( sToken );

		if( tt == ttOutOfData )
			return;

		switch( iState )
		{
			case 0:		// Start token, should be "method" (get, put, etc)
				SDB( 0 );
				sMethod = sToken;
				iState = 1;
				break;

			case 1:		// The path requested
				SDB( 1 );
				sPath = sToken;
				iState = 2;
				break;

			case 2:		// The protocol name and version
				SDB( 2 );
				if( strncmp( sToken.getStr(), "HTTP/", 5 ) )
				{
					pClient->disconnect();
					return;
				}
				else
				{
					char *s, *s2;
					s = sToken.getStr()+5;
					iMajor = strtol( s, &s2, 10 );
					iMinor = strtol( s2+1, NULL, 10 );
					iState = 3;
				}
				break;

			case 3:		// End of initial header, now comes mime-style blocks.
				SDB( 3 );
				if( tt == ttNewline )
				{
					iState = 10;
				}
				else if( tt == ttDoubleNewline )
				{
					earlyResponse();
				}
				else
				{
					pClient->disconnect();
					return;
				}
				break;

			case 10:		// HTTP-Message (skipped for now...)
				SDB( 10 );
				if( tt == ttString )
				{
					iState = 11;
				}
				else
				{
					pClient->disconnect();
				}
				break;

			case 11:		// Should be a colon...
				SDB( 11 );
				if( tt == ttSeperator && sToken == ":" )
				{
					iState = 12;
				}
				else
				{
					pClient->disconnect();
				}
				break;

			case 12:
				SDB( 12 );
				if( tt == ttNewline )
				{
					iState = 10;
				}
				if( tt == ttDoubleNewline )
				{
					earlyResponse();
				}
				break;

			case 20:
				SDB( 20 );
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
			if( pClient->getInputSize() < 4 )
				return ttOutOfData;
			
			char ss[3];
			pClient->peek( ss, 3, j+1 );
			if( ss[0] == LF && ss[1] != ' ' && ss[1] != '\t' )
			{
				if( bNonWS )
				{
					pClient->seek( j );
					return ttString;
				}
				else if( ss[1] == CR && ss[2] == LF )
				{
					pClient->seek( 4 );
					return ttDoubleNewline;
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

void Bu::ProtocolHttp::earlyResponse()
{
	if( sMethod == "GET" )
	{
		onRequest( sMethod, sPath );
		iState = 0;
	}
	else
	{
		iState = 20;
	}
}

void Bu::ProtocolHttp::lateResponse()
{
	onRequest( sMethod, sPath );
}

void Bu::ProtocolHttp::sendResponse( const Response &rRes )
{
	char buf[1024];
	int iSize = sprintf( buf, "HTTP/1.1 %d ", rRes.iCode );

	pClient->write( buf, iSize );
	pClient->write( rRes.sReason );
	pClient->write( CRLF, 2 );

	for( Response::StringHash::const_iterator i = rRes.hHeaders.begin();
		 i != rRes.hHeaders.end(); i++ )
	{
		pClient->write( i.getKey() );
		pClient->write(": ", 2 );
		pClient->write( i.getValue() );
		pClient->write( CRLF, 2 );
	}

	iSize = sprintf( buf, "Content-Length: %ld" CRLF, rRes.sContent.getSize() );
	pClient->write( buf, iSize );

	pClient->write( CRLF, 2 );
	pClient->write( rRes.sContent );
}

//
// Bu::ProtocolHttp::Response
//
Bu::ProtocolHttp::Response::Response( int iCode ) :
	iCode( iCode )
{
	switch( iCode )
	{
		case 100:	sReason = "Continue"; break;
		case 101:	sReason = "Switching Protocols"; break;
		case 200:	sReason = "OK"; break;
		case 201:	sReason = "Created"; break;
		case 202:	sReason = "Accepted"; break;
		case 203:	sReason = "Non-Authoritative Information"; break;
		case 204:	sReason = "No Content"; break;
		case 205:	sReason = "Reset Content"; break;
		case 206:	sReason = "Partial Content"; break;
		case 300:	sReason = "Multiple Choices"; break;
		case 301:	sReason = "Moved Permanently"; break;
		case 302:	sReason = "Found"; break;
		case 303:	sReason = "See Other"; break;
		case 304:	sReason = "Not Modified"; break;
		case 305:	sReason = "Use Proxy"; break;
		case 307:	sReason = "Temporary Redirect"; break;
		case 400:	sReason = "Bad Request"; break;
		case 401:	sReason = "Unauthorized"; break;
		case 402:	sReason = "Payment Required"; break;
		case 403:	sReason = "Forbidden"; break;
		case 404:	sReason = "Not Found"; break;
		case 405:	sReason = "Method Not Allowed"; break;
		case 406:	sReason = "Not Acceptable"; break;
		case 407:	sReason = "Proxy Authentication Required"; break;
		case 408:	sReason = "Request Time-out"; break;
		case 409:	sReason = "Conflict"; break;
		case 410:	sReason = "Gone"; break;
		case 411:	sReason = "Length Required"; break;
		case 412:	sReason = "Precondition Failed"; break;
		case 413:	sReason = "Request Entity Too Large"; break;
		case 414:	sReason = "Request-URI Too Large"; break;
		case 415:	sReason = "Unsupported Media Type"; break;
		case 416:	sReason = "Requested range not satisfiable"; break;
		case 417:	sReason = "Expectation Failed"; break;
		case 500:	sReason = "Internal Server Error"; break;
		case 501:	sReason = "Not Implemented"; break;
		case 502:	sReason = "Bad Gateway"; break;
		case 503:	sReason = "Service Unavailable"; break;
		case 504:	sReason = "Gateway Time-out"; break;
		case 505:	sReason = "HTTP Version not supported"; break;
	}
}

Bu::ProtocolHttp::Response::Response( int iCode, const Bu::FString &sReason ) :
	iCode( iCode ),
	sReason( sReason )
{
}

Bu::ProtocolHttp::Response::~Response()
{
}

void Bu::ProtocolHttp::Response::setHeader(
	const Bu::FString &sKey, const Bu::FString &sVal )
{
	hHeaders.insert( sKey, sVal );
}

void Bu::ProtocolHttp::Response::setContent( const Bu::FString &sCont )
{
	sContent = sCont;
}


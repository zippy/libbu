#include <string.h>
#include <stdlib.h>
#include "http.h"
#include "hashfunctionstring.h"

Http::Http( Connection *pConnection ) : hReqHeader( new HashFunctionString(), 100 )
{
	pCon = pConnection;
	nParseState = parseInit;
}

Http::~Http()
{
	for( int j = 0; j < lStrings.getSize(); j++ )
	{
		delete (std::string *)lStrings[j];
	}
}

bool Http::parseRequest()
{
	for(;;)
	{
		pCon->readInput();
		switch( nParseState )
		{
			case parseInit:
				{
					int nLen = pCon->scanInputFor( CR );
					if( nLen == -1 )
					{
						return false;
					}
					else
					{
						nReqType = getRequestType( pCon->getInput() );
						pCon->usedInput( pCon->scanInputFor(' ')+1 );

						nLen = pCon->scanInputFor(' ');
						sReqURI.append( pCon->getInput(), nLen );
						pCon->usedInput( nLen+1 );
					
						if( !strncmp( pCon->getInput(), "HTTP/", 5 ) )
						{
							char mbuf[2]={'\0','\0'};
							unsigned char major, minor;
							
							pCon->usedInput( 5 );
							mbuf[0] = pCon->getInput()[0];
							major = (unsigned char)atoi(mbuf);
							mbuf[0] = pCon->getInput()[2];
							minor = (unsigned char)atoi(mbuf);
							setRequestVersion( major, minor );
							if( checkRequestVer() )
							{
								nParseState = parseHeader;
							}
							else
							{
								setResponseStatus( statusHTTPVersionNotSupported );
								printf("Verson not supported.\n");
								return true;
							}
							
							pCon->usedInput( 5 );
						}
						else
						{
							setResponseStatus( statusBadRequest );
						}

						//return false;
					}
				}
				break;

			case parseHeader:
				{
					int nLen = pCon->scanInputFor( CR );
					if( nLen == -1 )
					{
						return false;
					}
					else if( nLen == 0 )
					{
						// We've got our double-newline, time for content.
						pCon->usedInput( 2 );
						setResponseStatus( statusOK );
						return true;
					}
					else
					{
						nLen = pCon->scanInputFor(':');
						if( nLen == -1 )
						{
							printf("No colon?  what are you trying to pull?\n");
						}
						else
						{
							std::string *pName = new std::string( pCon->getInput(), nLen );
							lStrings.append( pName );
							pCon->usedInput( nLen+1 );
							
							nLen = pCon->scanInputFor( CR );
							std::string *pValue = convSpaceString( pCon->getInput(), nLen );
							lStrings.append( pValue );
							pCon->usedInput( nLen+2 );
							
							hReqHeader.insert(
								pName->c_str(),
								pValue->c_str()
								);

							printf("::%s = \"%s\"\n",
								pName->c_str(),
								pValue->c_str()
								);
						}
					}
				}
				break;

			case parseFinished:
				break;
		}
	}
}

bool Http::buildResponse( short nResponseCode, const char *sResponse )
{
	if( nResponseCode > 0 )
	{
		nResStatus = nResponseCode;
	}

	if( sResponse == NULL )
	{
		sResStatusStr = "uh yeah";
	}
	else
	{
		sResStatusStr = sResponse;
	}

	time_t curTime;
	time( &curTime );
	gmtime_r( &curTime, &tResTime );

	sServerStr = "YFHttp/0.0.1";
	bResPersistant = false;

	//char buf[30];
	//strftime( buf, 30, "%a, %d %b %Y %H:%M:%S GMT", &tResponseTime );

	return true;
}

bool Http::sendResponse()
{
	char buf[256];

	sprintf( buf, "HTTP/1.1 %d %s\r\n", nResStatus, sResStatusStr.c_str() );
	pCon->appendOutput( buf );

	strftime( buf, 256, "Date: %a, %d %b %Y %H:%M:%S GMT\r\n", &tResTime );
	pCon->appendOutput( buf );

	sprintf( buf, "Server: %s\r\n", sServerStr.c_str() );
	pCon->appendOutput( buf );

	if( bResPersistant )
	{
	}
	else
	{
		pCon->appendOutput("Connection: close\r\n");
	}

	sprintf( buf, "Content-Type: %s\r\n", sResMime.c_str() );
	pCon->appendOutput( buf );
	
	sprintf( buf, "Content-Length: %d\r\n", sResContent.size() );
	pCon->appendOutput( buf );

	pCon->appendOutput("\r\n");

	pCon->appendOutput( sResContent.c_str(), sResContent.size() );

	return true;
}

void Http::setResponsePersistant( bool bPersistant )
{
	bResPersistant = bPersistant;
}

void Http::setResponseContent( const char *sMime, const char *sContent, int nLen )
{
	sResMime = sMime;
	sResContent.erase();
	sResContent.append( sContent, nLen );
}

std::string *Http::convSpaceString( const char *sStr, int nLen )
{
	int nNewLen = 0;
	bool bStart = true;
	bool bSpace = false;

	for( int j = 0; j < nLen; j++ )
	{
		if( sStr[j] == ' ' || sStr[j] == '\t' )
		{
			if( bStart )
			{
			}
			else if( bSpace == false )
			{
				bSpace = true;
				nNewLen++;
			}
		}
		else
		{
			bStart = false;
			bSpace = false;
			nNewLen++;
		}
	}
	if( bSpace )
	{
		nNewLen--;
	}

	std::string *pSStr = new std::string;
	//char *pStr = pSStr->c_str();
	nNewLen = 0;
	bStart = true;
	bSpace = false;

	for( int j = 0; j < nLen; j++ )
	{
		if( sStr[j] == ' ' || sStr[j] == '\t' )
		{
			if( bStart )
			{
			}
			else if( bSpace == false )
			{
				bSpace = true;
				*pSStr += ' ';
				//pStr[nNewLen++] = ' ';
			}
		}
		else
		{
			bStart = false;
			bSpace = false;
			*pSStr += sStr[j];
			//pStr[nNewLen++] = sStr[j];
		}
	}
	if( bSpace == true )
	{
		nNewLen--;
//		pStr[nNewLen] = '\0';
	}

	return pSStr;
}

const char *Http::getRequestURI()
{
	return sReqURI.c_str();
}

short Http::getRequestType( const char *sType )
{
	if( !strncmp( sType, "OPTIONS", 7 ) )
	{
		return reqOptions;
	}
	else if( !strncmp( sType, "GET", 3 ) )
	{
		return reqGet;
	}
	else if( !strncmp( sType, "HEAD", 4 ) )
	{
		return reqHead;
	}
	else if( !strncmp( sType, "POST", 4 ) )
	{
		return reqPost;
	}
	else if( !strncmp( sType, "PUT", 3 ) )
	{
		return reqPut;
	}
	else if( !strncmp( sType, "DELETE", 6 ) )
	{
		return reqDelete;
	}
	else if( !strncmp( sType, "TRACE", 5 ) )
	{
		return reqTrace;
	}
	else if( !strncmp( sType, "CONNECT", 7 ) )
	{
		return reqConnect;
	}
	else
	{
		printf(" Uh oh, extension!\n");
		return reqExtension;
	}
}

const char *Http::getRequestType( short nType )
{
	switch( nType )
	{
		case reqOptions:	return "OPTIONS";
		case reqGet:		return "GET";
		case reqHead:		return "HEAD";
		case reqPost:		return "POST";
		case reqPut:		return "PUT";
		case reqDelete:		return "DELETE";
		case reqTrace:		return "TRACE";
		case reqConnect:	return "CONNECT";
		case reqExtension:	return "EXTENSION";
		default:			return "INVALID VALUE";
	}
}

short Http::getRequestType()
{
	return nReqType;
}

const char *Http::getRequestTypeStr()
{
	return getRequestType( nReqType );
}

void Http::setResponseStatus( short nStatus )
{
	nResStatus = nStatus;
}

void Http::setRequestVersion( unsigned char nMajor, unsigned char nMinor )
{
	cReqVersion = (nMajor<<4)|nMinor;
}

unsigned char Http::getRequestMinorVer()
{
	return cReqVersion&0x0F;
}

unsigned char Http::getRequestMajorVer()
{
	return cReqVersion>>4;
}

bool Http::checkRequestVer()
{
	if( cReqVersion == HTTP11 )
		return true;
	return false;
}


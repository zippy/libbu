#include "bu/logger.h"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

Bu::Logger::Logger()
{
}

Bu::Logger::~Logger()
{
}

void Bu::Logger::log( int nLevel, const char *sFile, const char *sFunction, int nLine, const char *sFormat, ...)
{
	if( (nLevel&nLevelMask) == 0 )
		return;

	va_list ap;
	va_start( ap, sFormat );
	char *text;
	vasprintf( &text, sFormat, ap );
	va_end(ap);
	
	time_t t = time(NULL);
	
	char *line = NULL;
	struct tm *pTime;
	pTime = localtime( &t );
	asprintf(
		&line,
		sLogFormat.getStr(),
		pTime->tm_year+1900,
		pTime->tm_mon+1,
		pTime->tm_mday,
		pTime->tm_hour,
		pTime->tm_min,
		pTime->tm_sec,
		nLevel,
		sFile,
		nLine,
		text,
		sFunction
		);
	write( fileno(stdout), line, strlen(line) );
	free( text );
	free( line );
}

void Bu::Logger::setFormat( const Bu::FString &str )
{
	sLogFormat = "";

	static char fmts[][4]={
		{'y', 'd', '0', '1'},
		{'m', 'd', '0', '2'},
		{'d', 'd', '0', '3'},
		{'h', 'd', '0', '4'},
		{'M', 'd', '0', '5'},
		{'s', 'd', '0', '6'},
		{'L', 'd', '0', '7'},
		{'f', 's', '0', '8'},
		{'l', 'd', '0', '9'},
		{'t', 's', '1', '0'},
		{'F', 's', '1', '1'},
		{'\0', '\0', '\0', '\0'},
	};
	
	for( const char *s = str.getStr(); *s; s++ )
	{
		if( *s == '%' )
		{
			sLogFormat += '%';
			Bu::FString sBuf;
			for(;;)
			{
				s++;
				int l;
				for( l = 0;; l++ )
				{
					if( fmts[l][0] == '\0' )
					{
						sBuf += *s;
						break;
					}
					else if( *s == fmts[l][0] )
					{
						sLogFormat += fmts[l][2];
						sLogFormat += fmts[l][3];
						sLogFormat += '$';
						sLogFormat += sBuf;
						sLogFormat += fmts[l][1];
						break;
					}
				}
				if( fmts[l][0] != '\0' )
					break;
			}
		}
		else
		{
			sLogFormat += *s;
		}
	}
	sLogFormat += '\n';

	//write( fileno(stdout), sLogFormat.getStr(), sLogFormat.getSize() );
}

void Bu::Logger::setMask( int n )
{
	nLevelMask = n;
}

void Bu::Logger::setLevel( int n )
{
	int j;
	for( j = 31; j > 0; j-- )
	{
		if( (n&(1<<j)) )
		{
			for(; j >= 0; j-- )
			{
				n |= (1<<j);
			}
			nLevelMask = n;
			return;
		}
	}
}

void Bu::Logger::hexDump( int nLevel, const char *sFile, const char *sFunction,
		int nLine, const void *pDataV, long nDataLen,
		const char *lpName )
{
	log( nLevel, sFile, sFunction, nLine, "Displaying %ld bytes of %s.\n", nDataLen, lpName );
	const unsigned char *pData = (const unsigned char *)pDataV;
	int j = 0;
	Bu::FString sBorder;
	for( int l = 0; l < 8*3+2*8+2; l++ ) sBorder += ((l!=8*3)?("-"):("+"));
	sBorder += '\n';
	log( nLevel, sFile, sFunction, nLine, sBorder.getStr() );
	Bu::FString sLine;
	for(;;)
	{
		int kmax = 8;
		if( nDataLen-j < 8 ) kmax = nDataLen-j;
		for(int k = 0; k < 8; k++ )
		{
			if( k < kmax )
			{
				char buf[4];
				sprintf( buf, "%02X ", (int)((unsigned char)pData[j+k]) );
				sLine += buf;
			}
			else
			{
				sLine += "-- ";
			}
		}
		sLine += "| ";
		for(int k = 0; k < kmax; k++ )
		{
			char buf[3];
			sprintf( buf, "%c ", (pData[j+k]>32 && pData[j+k]<=128)?(pData[j+k]):('.') );
			sLine += buf;
		}
		sLine += '\n';
		log( nLevel, sFile, sFunction, nLine, sLine.getStr() );
		sLine = "";
		j += kmax;
		if( j >= nDataLen ) break;
	}
	log( nLevel, sFile, sFunction, nLine, sBorder.getStr() );
}

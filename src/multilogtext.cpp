#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "multilogtext.h"

bool fileexists( const char *sPath )
{
	int nFileDesc = open( sPath, O_RDONLY );
	if( nFileDesc < 0 )
	{
		return false;
	}
	else
	{
		close( nFileDesc );
		return true;
	}
}

MultiLogText::MultiLogText( const char *sFileName, const char *lpFormat, bool bRotateLog, int nMaxLogs )
{
	this->lpFormat = NULL;
	
	if( bRotateLog )
	{
		if( fileexists( sFileName ) == false )
		{
			return;
		}
	
		int nLen = strlen( sFileName );
		char *buf = new char[nLen+6];
		sprintf( buf, "%s.", sFileName );

		for( int j = 1; j < nMaxLogs; j++ )
		{
			sprintf( &buf[nLen+1], "%d", j );
			if( !fileexists( buf ) )
			{
				rename( sFileName, buf );
				break;
			}
		}
	}

	nFD = open( sFileName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH );
	setLogFormat( lpFormat );
}

MultiLogText::MultiLogText( int nFileDesc, const char *lpFormat )
{
	this->lpFormat = NULL;
	nFD = nFileDesc;
	setLogFormat( lpFormat );
}

MultiLogText::~MultiLogText()
{
	if( nFD != -1 )
	{
		close( nFD );
	}
	
	delete[] lpFormat;
}

bool MultiLogText::setLogFormat( const char *lpFormat )
{
	char buf[200];
	int k = 0;
	static char fmts[10][4]={
		{'y', 'd', '0', '1'},
		{'m', 'd', '0', '2'},
		{'d', 'd', '0', '3'},
		{'h', 'd', '0', '4'},
		{'M', 'd', '0', '5'},
		{'s', 'd', '0', '6'},
		{'l', 'd', '0', '7'},
		{'f', 's', '0', '8'},
		{'L', 'd', '0', '9'},
		{'t', 's', '1', '0'},
	};
	
	for( int j = 0; lpFormat[j] != '\0'; j++ )
	{
		if( lpFormat[j] == '%' )
		{
			buf[k++] = '%';
			int nPlace = k++;
			k++;
			buf[k++] = '$';
			bool bDone = false;
			for( j++; bDone == false; j++ )
			{
				int l;
				for( l = 0; l < 10; l++ )
				{
					if( lpFormat[j] == fmts[l][0] )
					{
						buf[nPlace] = fmts[l][2];
						buf[nPlace+1] = fmts[l][3];
						buf[k++] = fmts[l][1];
						bDone = true;
						break;
					}
				}
				if( l == 10 )
				{
					buf[k++] = lpFormat[j];
				}
			}
			j--;
		}
		else
		{
			buf[k++] = lpFormat[j];
		}
	}
	buf[k++] = '\n';
	buf[k] = '\0';
	
	if( this->lpFormat != NULL )
	{
		delete[] this->lpFormat;
	}
	this->lpFormat = new char[k+1];
	strcpy( this->lpFormat, buf );

	return true;
}

bool MultiLogText::openLog()
{
	if( nFD == -1 )
	{
		return false;
	}
	return true;
}

bool MultiLogText::append( MultiLog::LogEntry *pEntry )
{
	if( nFD == -1 )
	{
		return false;
	}

	char *line = NULL;
	struct tm *pTime;
	pTime = localtime( &pEntry->xTime );
	asprintf(
		&line,
		lpFormat,
		pTime->tm_year+1900,
		pTime->tm_mon+1,
		pTime->tm_mday,
		pTime->tm_hour,
		pTime->tm_min,
		pTime->tm_sec,
		pEntry->nLevel,
		pEntry->lpFile,
		pEntry->nLine,
		pEntry->lpText
		);
	write( nFD, line, strlen(line) );
	free( line );

	return true;
}

bool MultiLogText::closeLog()
{
	if( nFD == -1 )
	{
		return false;
	}
	// Don't close it if it's sdtout or errorout
	if( nFD > 2 )
	{
		close( nFD );
	}
	nFD = -1;
	return true;
}


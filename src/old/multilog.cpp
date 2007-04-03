#include "multilog.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "multilogchannel.h"

MultiLog::MultiLog()
{
	lChannel = new LinkedList();
	rEntry = new RingList( 150 );
	nEntriesLost = 0;
}

MultiLog::~MultiLog()
{
	int nMax = lChannel->getSize();
	for( int j = 0; j < nMax; j++ )
	{
		((MultiLogChannel *)lChannel->getAt(j))->closeLog();
		delete ((MultiLogChannel *)lChannel->getAt(j));
	}
	delete lChannel;
	
	for( int j = 0; j < rEntry->getSize(); j++ )
	{
		delete (LogEntry *)rEntry->getAt( j );
	}
	delete rEntry;
}
/*
void MultiLog::Log( int nLevel, const char *lpFormat, ...)
{
	switch( nLevel )
	{
		default:
			break;
	}
	va_list ap;
	va_start(ap, lpFormat);

	vprintf( lpFormat, ap );

	va_end(ap);
}*/

void MultiLog::DetailLog( int nLevel, const char *lpFile, int nLine, const char *lpFunction, const char *lpFormat, ...)
{
	LogEntry *e = new LogEntry();

	va_list ap;
	va_start(ap, lpFormat);
	char *text;
	vasprintf( &text, lpFormat, ap );
	va_end(ap);
	
	time( &e->xTime );
	e->nLevel = nLevel;
	e->nLine = nLine;
	e->lpFile = new char[strlen(lpFile)+1];
	strcpy( e->lpFile, lpFile );
	e->lpText = new char[strlen(text)+1];
	strcpy( e->lpText, text );
	free( text );
	
	append( e );
}

void MultiLog::append( LogEntry *pEntry )
{
	rEntry->append( pEntry );
	if( rEntry->getPushBuf() )
	{
		delete (LogEntry *)rEntry->getPushBuf();
		nEntriesLost++;
	}

	for( int j = 0; j < lChannel->getSize(); j++ )
	{
		((MultiLogChannel *)lChannel->getAt( j ))->append( pEntry );
	}
}

void MultiLog::addChannel( MultiLogChannel *pChannel )
{
	lChannel->append( pChannel );
	
	pChannel->openLog();
	
	for( int j = 0; j < rEntry->getSize(); j++ )
	{
		pChannel->append( (LogEntry *)rEntry->getAt( j ) );
	}
}

MultiLog::LogEntry::~LogEntry()
{
	delete[] lpFile;
	delete[] lpText;
}


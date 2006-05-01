/***************************************************************************
                          multilog.cpp  -  description
                             -------------------
    begin                : Sat Sep 6 2003
    copyright            : (C) 2003 by Mike Buland
    email                : eichlan@yf-soft.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "multilog.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "multilogchannel.h"

// This section is what we need to make this a singleton
// this makes this class easy to use from anywhere, without
// worrying about re-creating every output form and all of that crazy jazz
MultiLog *MultiLog::singleLog = NULL;

MultiLog *MultiLog::getLog()
{
	if( singleLog == NULL )
	{
		singleLog = new MultiLog;
		atexit( cleanup );
	}
	return singleLog;
}

void MultiLog::cleanup()
{
	if( singleLog != NULL )
	{
		delete singleLog;
		singleLog = NULL;
	}
}

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


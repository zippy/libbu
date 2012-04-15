/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/taf.h"
#include "bu/string.h"
#include "bu/stream.h"

#include <stdlib.h>

using namespace Bu;

Bu::TafReader::TafReader( Bu::Stream &sIn ) :
	c( 0 ),
	la( 0 ),
	sIn( sIn ),
	iLine( 1 ), iCol( -1 )
{
	next(); next();
}

Bu::TafReader::~TafReader()
{

}

Bu::TafGroup *Bu::TafReader::readGroup()
{
	ws();
	if( c != '{' )
		throw TafException("%d:%d: Expected '{' got '%c'.", iLine, iCol, c );
	next();
	ws();
	String sName = readStr();
	TafGroup *pGroup = new TafGroup( sName );
	try
	{
		ws();
		if( c != ':' )
			throw TafException("%d:%d: Expected ':' got '%c'.",
				iLine, iCol, c );
		next();
		//printf("Node[%s]:\n", sName.getStr() );

		groupContent( pGroup );

		if( c != '}' )
			throw TafException("%d:%d: Expected '}' got '%c'.",
				iLine, iCol, c );

		//next();

		return pGroup;
	}
	catch(...)
	{
		delete pGroup;
		throw;
	}
}

void Bu::TafReader::groupContent( Bu::TafGroup *pGroup )
{
	for(;;)
	{
		ws();
		if( c == '{' )
		{
			pGroup->addChild( readGroup() );
			next();
		}
		else if( c == '}' )
			return;
		else if( c == '/' && la == '*' )
			pGroup->addChild( readComment() );
		else if( c == '/' && la == '/' )
			pGroup->addChild( readComment( true ) );
		else if( c == ':' )
			throw TafException("%d:%d: Encountered stray ':' in taf stream.",
				iLine, iCol );
		else
			pGroup->addChild( readProperty() );
	}
}

Bu::TafProperty *Bu::TafReader::readProperty()
{
	String sName = readStr();
	ws();
	if( c != '=' )
	{
		//printf("  %s (true)\n", sName.getStr() );
		return new Bu::TafProperty( "", sName );
	}
	next();
	String sValue = readStr();
	return new Bu::TafProperty( sName, sValue );
	//printf("  %s = %s\n", sName.getStr(), sValue.getStr() );
}

Bu::TafComment *Bu::TafReader::readComment( bool bEOL )
{
	String sCmnt;
	next();
	if( bEOL )
	{
		for(;;)
		{
			next();
			if( c == '\n' && la == '\r' )
			{
				next(); next();
				break;
			}
			else if( c == '\n' || c == '\r' )
			{
				next();
				break;
			}
			sCmnt += c;
		}
	}
	else
	{
		for(;;)
		{
			next();
			if( c == '*' && la == '/' )
			{
				next(); next();
				break;
			}
			sCmnt += c;
		}
	}

	return new TafComment( sCmnt, bEOL );
}

Bu::String Bu::TafReader::readStr()
{
	ws();
	String s;
	if( c == '"' )
	{
		next();
		for(;;)
		{
			if( c == '\\' )
			{
				next();
				if( c == 'x' )
				{
					char code[3]={'\0','\0','\0'};
					next();
					code[0] = c;
					next();
					code[1] = c;
					c = (unsigned char)strtol( code, NULL, 16 );
				}
				else if( c == '"' )
					c = '"';
				else if( c == '\\' )
					c = '\\';
				else if( c == 'n' )
					c = '\n';
				else if( c == 't' )
					c = '\t';
				else
					throw TafException("%d:%d: Invalid escape sequence '\\%c'.",
						iLine, iCol, c );
			}
			else if( c == '"' )
				break;
			s += c;
			next();
		}
		next();
	}
	else
	{
		for(;;)
		{
			if( isws() || c == '}' || c == '{' || c == ':' || c == '=' )
				break;
			s += c;
			next();
		}
	}

	return s;
}

void Bu::TafReader::ws()
{
	for(;;)
	{
		if( !isws() )
			return;

		next();
	}
}

bool Bu::TafReader::isws()
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

void Bu::TafReader::next()
{
	if( c == '\n' )
	{
		iLine++;
		iCol = 1;
	}
	else
		iCol++;
	if( c == '}' )
	{
		rawread( &c );
		if( c != '}' )
			rawread( &la );
	}
	else
	{
		c = la;
		if( c != '}' )
			rawread( &la );
	}
}

void Bu::TafReader::rawread( char *c )
{
	if( sIn.read( c, 1 ) < 1 )
	{
		if( sIn.isEos() )
		{
			throw TafException("%d:%d: Premature end of stream.",
				iLine, iCol, c );
		}
		else
		{
			throw TafException("%d:%d: No data read, but not end of stream?",
				iLine, iCol, c );
		}
	}
}


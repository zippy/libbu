/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/tafreader.h"
#include "bu/exceptions.h"
#include "bu/fstring.h"

using namespace Bu;

Bu::TafReader::TafReader( Bu::Stream &sIn ) :
	c( 0 ),
	sIn( sIn )
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
		throw TafException("Expected '{'");
	next();
	ws();
	FString sName = readStr();
	TafGroup *pGroup = new TafGroup( sName );
	next();
	//printf("Node[%s]:\n", sName.getStr() );

	groupContent( pGroup );

	if( c != '}' )
		throw TafException("Expected '}'");

	next();

	return pGroup;
}

void Bu::TafReader::groupContent( Bu::TafGroup *pGroup )
{
	for(;;)
	{
		ws();
		if( c == '{' )
			pGroup->addChild( readGroup() );
		else if( c == '}' )
			return;
		else if( c == '/' && la == '*' )
			pGroup->addChild( readComment() );
		else if( c == '/' && la == '/' )
			pGroup->addChild( readComment( true ) );
		else if( c == ':' )
			throw TafException("Encountered stray ':' in taf stream.");
		else
			pGroup->addChild( readProperty() );
	}
}

Bu::TafProperty *Bu::TafReader::readProperty()
{
	FString sName = readStr();
	ws();
	if( c != '=' )
	{
		//printf("  %s (true)\n", sName.getStr() );
		return new Bu::TafProperty( "", sName );
	}
	next();
	FString sValue = readStr();
	return new Bu::TafProperty( sName, sValue );
	//printf("  %s = %s\n", sName.getStr(), sValue.getStr() );
}

Bu::TafComment *Bu::TafReader::readComment( bool bEOL )
{
	FString sCmnt;
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

Bu::FString Bu::TafReader::readStr()
{
	ws();
	FString s;
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
				else
					throw TafException("Invalid escape sequence.");
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
	c = la;
	sIn.read( &la, 1 );
}


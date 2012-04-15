/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/minimacro.h"

Bu::MiniMacro::MiniMacro()
{
	hFuncs.insert("toupper", new FuncToUpper() );
	hFuncs.insert("tolower", new FuncToLower() );
}

Bu::MiniMacro::MiniMacro( const StrHash &sVarSrc )
{
	for( StrHash::const_iterator i = sVarSrc.begin(); i != sVarSrc.end(); i++ )
	{
		addVar( i.getKey(), i.getValue() );
	}
}

Bu::MiniMacro::~MiniMacro()
{
}

Bu::String Bu::MiniMacro::parse( const Bu::String &sIn )
{
	bContinue = true;
	Bu::String sOut;
	for( sCur = sIn.getStr(); *sCur && bContinue; sCur++ )
	{
		if( *sCur == '{' )
		{
			switch( sCur[1] )
			{
				case '=':
					sCur += 2;
					sOut += parseRepl();
					break;

				case '?':
					sCur += 2;
					sOut += parseCond();
					break;

				case ':':
					sCur += 2;
					sOut += parseCmd();
					break;

				default:
					sOut += *sCur;
					continue;
			}
		}
		else
		{
			sOut += *sCur;
		}
	}

	iLastPos = (ptrdiff_t)sCur - (ptrdiff_t)sIn.getStr();

	return sOut;
}

Bu::String Bu::MiniMacro::parseRepl()
{
	Bu::String sOut;
	bool bIsFirst = true;
	for( const char *sNext = sCur;;)
	{
		for(; *sNext != ':' && *sNext != '}' && *sNext != '\0'; sNext++ ) { }
		if( *sNext == '\0' )
			break;
		Bu::String sName( sCur, (ptrdiff_t)sNext-(ptrdiff_t)sCur );
		if( bIsFirst )
		{
			sOut = hVars[sName];
			bIsFirst = false;
			//printf("Variable: \"%s\"\n", sName.getStr() );
		}
		else
		{
			sOut = callFunc( sOut, sName );
			//printf("Filter: \"%s\"\n", sName.getStr() );
		}
		if( *sNext == '}' )
		{
			sCur = sNext;
			break;
		}
		else if( *sNext == ':' )
		{
		}
		sNext++;
		sCur = sNext;
	}
	return sOut;
}

Bu::String Bu::MiniMacro::parseCond()
{
	Bu::String sOut;
	//printf("%20s\n", sCur );
	return sOut;
}

Bu::String Bu::MiniMacro::parseCmd()
{
	Bu::String sOut;
	const char *sNext = sCur;
	for(; *sNext != ':' && *sNext != '}' && *sNext != '\0'; sNext++ ) { }
	if( *sNext != '\0' )
	{
		Bu::String sName( sCur, (ptrdiff_t)sNext-(ptrdiff_t)sCur );
		if( sName == "end" )
		{
			sCur = sNext;
			bContinue = false;
			return sOut;
		}
		else
		{
			throw Bu::ExceptionBase("Unknown command '%s'.",
				sName.getStr()
				);
		}
	}
	else
	{
		//printf("Uh...?\n");
	}

	//printf("%20s\n", sCur );
	return sOut;
}

Bu::String Bu::MiniMacro::callFunc(
	const Bu::String &sIn, const Bu::String &sFunc )
{
	int i = sFunc.findIdx('(');
	if( i < 0 )
		throw Bu::ExceptionBase("That doesn't look like a function call");
	Bu::String sName( sFunc.getStr(), i );
	StrList lsParams;
	for( const char *s = sFunc.getStr()+i+1; *s && *s != ')'; s++ )
	{
		for(; *s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'; s++ ) { }
		const char *sNext;
		for( sNext = s; *sNext && *sNext != ')' && *sNext != ','; sNext++ ) { }
		Bu::String p( s, (ptrdiff_t)sNext-(ptrdiff_t)s );
		lsParams.append( p );
		sNext++;
		s = sNext;
	}
	return hFuncs.get( sName )->call( sIn, lsParams );
}

void Bu::MiniMacro::addVar(
	const Bu::String &sName, const Bu::String &sValue )
{
	hVars.insert( sName, sValue );
}

bool Bu::MiniMacro::hasVar( const Bu::String &sName )
{
	return hVars.has( sName );
}

const Bu::String &Bu::MiniMacro::getVar( const Bu::String &sName )
{
	return hVars.get( sName );
}

const Bu::StrHash &Bu::MiniMacro::getVars()
{
	return hVars;
}

int Bu::MiniMacro::getPosition()
{
	return iLastPos;
}


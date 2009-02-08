/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
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

Bu::FString Bu::MiniMacro::parse( const Bu::FString &sIn )
{
	bContinue = true;
	Bu::FString sOut;
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

Bu::FString Bu::MiniMacro::parseRepl()
{
	Bu::FString sOut;
	bool bIsFirst = true;
	for( const char *sNext = sCur;;)
	{
		for(; *sNext != ':' && *sNext != '}' && *sNext != '\0'; sNext++ ) { }
		if( *sNext == '\0' )
			break;
		Bu::FString sName( sCur, (ptrdiff_t)sNext-(ptrdiff_t)sCur );
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

Bu::FString Bu::MiniMacro::parseCond()
{
	Bu::FString sOut;
	//printf("%20s\n", sCur );
	return sOut;
}

Bu::FString Bu::MiniMacro::parseCmd()
{
	Bu::FString sOut;
	const char *sNext = sCur;
	for(; *sNext != ':' && *sNext != '}' && *sNext != '\0'; sNext++ ) { }
	if( *sNext != '\0' )
	{
		Bu::FString sName( sCur, (ptrdiff_t)sNext-(ptrdiff_t)sCur );
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

Bu::FString Bu::MiniMacro::callFunc(
	const Bu::FString &sIn, const Bu::FString &sFunc )
{
	int i = sFunc.findIdx('(');
	if( i < 0 )
		throw Bu::ExceptionBase("That doesn't look like a function call");
	Bu::FString sName( sFunc.getStr(), i );
	StrList lsParams;
	for( const char *s = sFunc.getStr()+i+1; *s && *s != ')'; s++ )
	{
		for(; *s == ' ' || *s == '\t' || *s == '\r' || *s == '\n'; s++ ) { }
		const char *sNext;
		for( sNext = s; *sNext && *sNext != ')' && *sNext != ','; sNext++ ) { }
		Bu::FString p( s, (ptrdiff_t)sNext-(ptrdiff_t)s );
		lsParams.append( p );
		sNext++;
		s = sNext;
	}
	return hFuncs.get( sName )->call( sIn, lsParams );
}

void Bu::MiniMacro::addVar(
	const Bu::FString &sName, const Bu::FString &sValue )
{
	hVars.insert( sName, sValue );
}

bool Bu::MiniMacro::hasVar( const Bu::FString &sName )
{
	return hVars.has( sName );
}

const Bu::FString &Bu::MiniMacro::getVar( const Bu::FString &sName )
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


/*
 * Copyright (C) 2007 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "xmlreader.h"

Bu::XmlReader::XmlReader( Bu::Stream &sIn ) :
	sIn( sIn )
{
}

Bu::XmlReader::~XmlReader()
{
}

const char *Bu::XmlReader::lookahead( int nAmnt )
{
	if( sBuf.getSize() >= nAmnt )
		return sBuf.getStr();

	int nNew = nAmnt - sBuf.getSize();
	char *buf = new char[nNew];
	sIn.read( buf, nNew );
	sBuf.append( buf );

	return sBuf.getStr();
}

void Bu::XmlReader::burn( int nAmnt )
{
	if( sBuf.getSize() < nAmnt )
	{
		lookahead( nAmnt );
	}

	//sBuf.remove( nAmnt );
}

void Bu::XmlReader::checkString( const char *str, int nLen )
{
	if( !strncmp( str, lookahead( nLen ), nLen ) )
	{
		burn( nLen );
		return;
	}

	throw Bu::ExceptionBase("Expected string '%s'", str );
}

Bu::XmlNode *Bu::XmlReader::read()
{
	prolog();
}

void Bu::XmlReader::prolog()
{
	XMLDecl();
	Misc();
}

void Bu::XmlReader::XMLDecl()
{
	checkString("<?xml", 5 );
	S();
	VersionInfo();
	EncodingDecl();
	SDDecl();
	Sq();
	checkString("?>", 2 );
}

void Bu::XmlReader::Misc()
{
	for(;;)
	{
		S();
		if( !strncmp("<!--", lookahead( 4 ), 4 ) )
		{
			Comment();
		}
		else if( !strncmp("<?", lookahead( 2 ), 2 ) )
		{
			PI();
		}
		else
		{
			return;
		}
	}
}

void Bu::XmlReader::Comment()
{
	checkString("<!--", 4 );
	for(;;)
	{
		unsigned char c = *lookahead(1);
		if( c == '-' )
		{
			if( lookahead(2)[1] == '-' )
			{
				checkString("-->", 3 );
				return;
			}
		}
		burn( 1 );
	}
}

void Bu::XmlReader::PI()
{
	checkString("<?", 2 );
	FString sName = Name();
	printf("PI: %s\n---\n", sName.getStr() );
	S();
	for(int j = 0;; j++ )
	{
		if( !strncmp( "?>", lookahead(j+2)+j, 2 ) )
		{
			burn( j+2 );
			return;
		}
	}
}

void Bu::XmlReader::S()
{
	for( int j = 0;; j++ )
	{
		char c = *lookahead( 1 );
		if( c == 0x20 || c == 0x9 || c == 0xD || c == 0xA )
			continue;
		if( j == 0 )
			throw ExceptionBase("Expected whitespace.");
		return;
	}
}

void Bu::XmlReader::Sq()
{
	for(;;)
	{
		char c = *lookahead( 1 );
		if( c == 0x20 || c == 0x9 || c == 0xD || c == 0xA )
			continue;
		return;
	}
}

void Bu::XmlReader::VersionInfo()
{
	try
	{
		S();
		checkString("version", 7 );
	}
	catch( ExceptionBase &e )
	{
		return;
	}
	Eq();
	Bu::FString ver = AttValue();
	if( ver != "1.1" )
		throw ExceptionBase("Currently we only support xml version 1.1\n");
}

void Bu::XmlReader::Eq()
{
	Sq();
	checkString("=", 1 );
	Sq();
}

void Bu::XmlReader::EncodingDecl()
{
	S();
	try
	{
		checkString("encoding", 8 );
	}
	catch( ExceptionBase &e )
	{
		return;
	}

	Eq();
	AttValue();
}

void Bu::XmlReader::SDDecl()
{
	S();
	try
	{
		checkString("standalone", 10 );
	}
	catch( ExceptionBase &e )
	{
		return;
	}

	Eq();
	AttValue();
}

Bu::FString Bu::XmlReader::AttValue()
{
	char q = *lookahead(1);
	if( q == '\"' )
	{
		for( int j = 2;; j++ )
		{
			if( lookahead(j)[j-1] == '\"' )
			{
				Bu::FString ret( lookahead(j)+1, j-2 );
				burn( j );
				return ret;
			}
		}
	}
	else if( q == '\'' )
	{
		for( int j = 2;; j++ )
		{
			if( lookahead(j)[j-1] == '\'' )
			{
				Bu::FString ret( lookahead(j)+1, j-2 );
				burn( j );
				return ret;
			}
		}
	}

	throw ExceptionBase("Excpected either \' or \".\n");
}

Bu::FString Bu::XmlReader::Name()
{
	unsigned char c = *lookahead( 1 );
	if( c != ':' && c != '_' &&
		(c < 'A' || c > 'Z') &&
		(c < 'a' || c > 'z') &&
		(c < 0xC0 || c > 0xD6 ) &&
		(c < 0xD8 || c > 0xF6 ) &&
		(c < 0xF8))
	{
		throw ExceptionBase("Invalid entity name starting character.");
	}

	for( int j = 1;; j++ )
	{
		unsigned char c = lookahead(j+1)[j];
		if( isS( c ) )
		{
			FString ret( lookahead(j+1), j+1 );
			burn( j+1 );
			return ret;
		}
		if( c != ':' && c != '_' && c != '-' && c != '.' && c != 0xB7 &&
			(c < 'A' || c > 'Z') &&
			(c < 'a' || c > 'z') &&
			(c < '0' || c > '9') && 
			(c < 0xC0 || c > 0xD6 ) &&
			(c < 0xD8 || c > 0xF6 ) &&
			(c < 0xF8))
		{
			throw ExceptionBase("Invalid character in name.");
		}
	}
}


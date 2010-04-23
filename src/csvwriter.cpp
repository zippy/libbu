/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/csvwriter.h"
#include "bu/stream.h"

Bu::CsvWriter::CsvWriter( Bu::Stream &sOut, Bu::CsvWriter::Style eStyle ) :
	sOut( sOut )
{
	switch( eStyle )
	{
		case styleExcel:
			sEncode = Bu::slot( &encodeExcel );
			break;

		case styleC:
			sEncode = Bu::slot( &encodeExcel );
			break;
	}
}

Bu::CsvWriter::CsvWriter( Bu::Stream &sOut,
		Bu::CsvWriter::EncodeSignal sEncode ) :
	sOut( sOut ),
	sEncode( sEncode )
{
}

Bu::CsvWriter::~CsvWriter()
{
}

void Bu::CsvWriter::writeLine( const StrArray &aStrs )
{
	Bu::FString sBuf;
	for( StrArray::const_iterator i = aStrs.begin(); i; i++ )
	{
		if( i != aStrs.begin() )
			sBuf += ",";
		sBuf += sEncode( *i );
	}
	sBuf += "\n";

	sOut.write( sBuf );
}

Bu::FString Bu::CsvWriter::encodeExcel( const Bu::FString &sIn )
{
	if( sIn.find('\"') )
	{
		Bu::FString sOut = "\"";
		for( Bu::FString::const_iterator i = sIn.begin(); i; i++ )
		{
			if( *i == '\"' )
				sOut += "\"\"";
			else
				sOut += *i;
		}
		return sOut;
	}
	return sIn;
}

Bu::FString Bu::CsvWriter::encodeC( const Bu::FString &sIn )
{
	Bu::FString sOut = "";
	for( Bu::FString::const_iterator i = sIn.begin(); i; i++ )
	{
		if( *i == ',' )
			sOut += "\\,";
		else
			sOut += *i;
	}
	return sOut;
}


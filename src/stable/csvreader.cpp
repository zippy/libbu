/*
 * Copyright (C) 2007-2013 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/csvreader.h"
#include "bu/stream.h"

#include "bu/sio.h"
using namespace Bu;

Bu::CsvReader::CsvReader( Bu::Stream &sIn, Bu::CsvReader::Style eStyle ) :
    sIn( sIn )
{
    switch( eStyle )
    {
        case styleExcel:
            sDecode = Bu::slot( &decodeExcel );
            break;

        case styleC:
            sDecode = Bu::slot( &decodeC );
            break;
    }
}

Bu::CsvReader::CsvReader( Bu::Stream &sIn,
        Bu::CsvReader::DecodeSignal sDecode ) :
    sIn( sIn ),
    sDecode( sDecode )
{
}

Bu::CsvReader::~CsvReader()
{
}

Bu::StrArray Bu::CsvReader::readLine()
{
    Bu::StrArray aVals;

    Bu::String sLine = sIn.readLine();

    if( !sLine.isSet() )
        return Bu::StrArray();

    Bu::String::iterator i = sLine.begin();

    aVals.append( sDecode( i ) );

    while( i )
    {
        if( *i == ',' )
        {
            i++;
            if( !i )
            {
                aVals.append("");
                break;
            }
            aVals.append( sDecode( i ) );
        }
        else
        {
            // Blanks and stuff?
            sio << "Out of bound:  '" << *i << "'" << sio.nl;
            i++;
        }
    }

    return aVals;
}

Bu::String Bu::CsvReader::decodeExcel( Bu::String::iterator &i )
{
    Bu::String sRet;

    for(; i && (*i == ' ' || *i == '\t'); i++ ) { }
    
    if( !i )
        return sRet;

    if( *i == '\"' )
	{
		for( i++ ; i; i++ )
		{
			if( *i == '\"' )
            {
                i++;
                if( !i )
                {
                    return sRet;
                }
                else if( *i == '\"' )
				{
					sRet += *i;
				}
				else
				{
					return sRet;
				}
			}
			else
			{
				sRet += *i;
			}
		}
	}
	else
	{
		for( ; i; i++ )
		{
			if( *i == ',' )
			{
				return sRet;
			}
			sRet += *i;
		}
	}

	return sRet;
}

Bu::String Bu::CsvReader::decodeC( Bu::String::iterator & )
{
	return "";
}


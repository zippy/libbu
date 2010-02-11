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
			sDecode = Bu::slot( &decodeExcel );
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

	Bu::FString sLine = sIn.readLine();

	for( Bu::FString::iterator i = sLine.begin(); i; i++ )
	{
		if( *i == ',' )
		{
		}
		else
		{
			aVals.append( sDecode( i ) );
		}
	}

	return aVals;
}

Bu::FString Bu::CsvReader::decodeExcel( Bu::FString::iterator &i )
{
	Bu::FString sRet;

	for(; i && (*i == ' ' || *i == '\t'); i++ ) { }
	
	if( *i == '\"' )
	{
		for( i++ ; i; i++ )
		{
			if( *i == '\"' )
			{
				i++;
				if( *i == '\"' )
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

Bu::FString Bu::CsvReader::decodeC( Bu::FString::iterator &i )
{
	return "";
}


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

Bu::FString Bu::CsvWriter::encodeExcel( const Bu::FString &sIn )
{
	return "";
}

Bu::FString Bu::CsvWriter::encodeC( const Bu::FString &sIn )
{
	return "";
}


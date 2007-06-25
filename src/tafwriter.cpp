#include "tafwriter.h"

Bu::TafWriter::TafWriter( Bu::Stream &sOut ) :
	sOut( sOut )
{
}

Bu::TafWriter::~TafWriter()
{
}

void Bu::TafWriter::writeNode( Bu::TafNode *pRoot )
{
	sOut.write("{", 1 );
	writeString( pRoot->getName().getStr() );
	sOut.write(": ", 2 );
	sOut.write("}", 1 );
}

void Bu::TafWriter::writeString( const Bu::FString &str )
{
	sOut.write("\"", 1 );
	for( const char *s = str.getStr(); *s; s++ )
	{
		if( *s == '\"' )
			sOut.write("\\\"", 2 );
		else
			sOut.write( s, 1 );
	}
	sOut.write("\"", 1 );
}


#include "tafreader.h"

Bu::TafReader::TafReader( Bu::Stream &sIn ) :
	sIn( sIn )
{
}

Bu::TafReader::~TafReader()
{
}

void Bu::TafReader::next()
{
	sIn.read( &c, 1 );
}


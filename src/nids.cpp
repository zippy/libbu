#include "bu/nids.h"

Bu::Nids::Nids( Bu::Stream &sStore ) :
	sStore( sStore )
{
}

Bu::Nids::~Nids()
{
}

void Bu::Nids::initialize( int iBlockSize, int iPreAllocate )
{
}

int Bu::Nids::createStream( int iPreAllocate )
{
	return 0;
}



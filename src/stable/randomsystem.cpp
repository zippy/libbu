#include "bu/randomsystem.h"
#include "bu/file.h"

Bu::RandomSystem::RandomSystem( Type eType ) :
	eType( eType ),
	pSrc( 0 )
{
	switch( eType )
	{
		case Bu::RandomSystem::Fast:
			pSrc = new Bu::File("/dev/urandom", Bu::File::Read );
			break;

		case Bu::RandomSystem::Good:
			pSrc = new Bu::File("/dev/random", Bu::File::Read );
			break;
	}
}

Bu::RandomSystem::~RandomSystem()
{
	delete pSrc;
}

void Bu::RandomSystem::seed( int32_t /*iSeed*/ )
{
	// Seed really has no effect here...
	// on linux, if we were root, we could write data to random/urandom to
	// perturb the data, but it's not necesarry
}

int32_t Bu::RandomSystem::rand()
{
	if( !pSrc )
		throw Bu::ExceptionBase("Not initialized");
	int32_t i;
	pSrc->read( &i, sizeof(int32_t) );
	return i;
}


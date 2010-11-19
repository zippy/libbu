#include "bu/myriadfs.h"
#include "bu/myriadstream.h"

#include <string.h>
#include <unistd.h>

namespace Bu { subExceptionDef( MyriadFsException ) }

#define Myriad_Fs_MAGIC_CODE		((char *)"\xa7\x18\x8b\x39")

Bu::MyriadFs::MyriadFs( Bu::Stream &rStore, int iBlockSize ) :
	rStore( rStore ),
	mStore( rStore, iBlockSize )
{
	if( mStore.hasStream( 1 ) )
	{
		// Check to see if this is a MyriadFs stream.
		Bu::MyriadStream ms = mStore.openStream( 1 );
		char sMagic[4];
		if( ms.read( sMagic, 4 ) < 4 )
			throw MyriadFsException("The provided stream does not appear to be "
				"a MyriadFs stream.");
		if( ::strncmp( sMagic, Myriad_Fs_MAGIC_CODE, 4 ) )
			throw MyriadFsException("The provided stream does not appear to be "
				"a MyriadFs stream.");
	
		int8_t iVer;
	}
	else
	{
		// Create initial header stream
		{
			mStore.createStream( 1 );
			Bu::MyriadStream ms = mStore.openStream( 1 );
			ms.write( Myriad_Fs_MAGIC_CODE, 4 );
			int8_t iVer = 1;
			int32_t iTmp = 1;
			ms.write( &iVer, 1 );
			ms.write( &iBlockSize, 4 );
			ms.write( &iTmp, 4 ); // iNumNodes
			iTmp = 0;
			ms.write( &iTmp, 4 ); // iInode
			ms.write( &iTmp, 0 ); // iPosition
		}

		// Create initial inode stream, with one root node.
		{
			mStore.createStream( 2 );
			Bu::MyriadStream ms = mStore.openStream( 2 );
			int32_t iUser = 0, iGroup = 0;
#ifndef WIN32
			iUser = getuid();
			iGroup = getgid();
#endif
			int32_t iTmp32 = 0;
			int16_t iTmp16 = 0;
			ms.write( &iUser, 4 );
			ms.write( &iGroup, 4 );
			ms.write( &iTmp16, 2 );
			ms.write( &iTmp16, 2 );
			iTmp32 = 3;
			ms.write( &iTmp32, 4 );
			iTmp32 = 0;
			ms.write( &iTmp32, 4 );
			ms.write( &iTmp16, 2 );	
		}

		// Create inode 0's storage stream.
		{
			mStore.createStream( 3 );
		}
	}
}

Bu::MyriadFs::~MyriadFs()
{
}


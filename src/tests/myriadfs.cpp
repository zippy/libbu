#include "bu/file.h"
#include "bu/membuf.h"
#include "bu/myriadfs.h"
#include "bu/myriadstream.h"
#include "bu/sio.h"

using namespace Bu;

int main( int argc, char *argv[] )
{
	Bu::MemBuf mb;
	Bu::MyriadFs mfs( mb, 512 );

	{
		Bu::MyriadStream ms = mfs.open("/hello", Bu::MyriadFs::Read );
		ms.write("world!");
	}

	{
		Bu::MyriadStream ms = mfs.open("/hello", Bu::MyriadFs::Read );
		char buf[512];
		buf[ms.read( buf, 512 )] = '\0';
		sio << "read: '" << buf << "'" << sio.nl;
	}
}


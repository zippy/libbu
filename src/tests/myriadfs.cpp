#include "bu/file.h"
#include "bu/membuf.h"
#include "bu/myriadfs.h"
#include "bu/myriadstream.h"
#include "bu/sio.h"

using namespace Bu;

int main()
{
//	Bu::MemBuf mb;
	Bu::File mb("store.myr", File::Read|File::Write|File::Create );
	Bu::MyriadFs mfs( mb, 512 );

	sio << "Creating dirs..." << sio.nl;
	mfs.create("/etc", Bu::MyriadFs::typeDir|0755 );
	mfs.create("/dev", Bu::MyriadFs::typeDir|0755 );
	mfs.create("/usr", Bu::MyriadFs::typeDir|0755 );

	mfs.create("/dev/null", Bu::MyriadFs::typeChrDev|0666, 1, 3 );
	mfs.create("/dev/zero", Bu::MyriadFs::typeChrDev|0666, 1, 5 );
	mfs.create("/dev/sda", Bu::MyriadFs::typeBlkDev|0660, 8, 0 );

	sio << "Creating files..." << sio.nl;
	{
		Bu::MyriadStream ms = mfs.open("/hello", Bu::MyriadFs::Read );
		ms.write("world!");
	}
	{
		Bu::MyriadStream ms = mfs.open("/etc/hello", Bu::MyriadFs::Read );
		ms.write("world, again!");
	}

	sio << "Reading files..." << sio.nl;
	{
		Bu::MyriadStream ms = mfs.open("/hello", Bu::MyriadFs::Read );
		char buf[512];
		buf[ms.read( buf, 512 )] = '\0';
		sio << "read: '" << buf << "'" << sio.nl;
	}
	{
		Bu::MyriadStream ms = mfs.open("/etc/hello", Bu::MyriadFs::Read );
		char buf[512];
		buf[ms.read( buf, 512 )] = '\0';
		sio << "read: '" << buf << "'" << sio.nl;
	}
}


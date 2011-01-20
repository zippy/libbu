#include "bu/udpsocket.h"
#include "bu/sio.h"

using namespace Bu;

int main( int argc, char *argv[] )
{
	if( argv[1][0] == 'l' )
	{
		sio << "Listening..." << sio.nl;
		Bu::UdpSocket udp( "255.255.255.255", 6688, UdpSocket::Read|UdpSocket::Broadcast );

		for(;;)
		{
			char buf[1501];
			int iRead = udp.read( buf, 1500 );
			buf[iRead] = '\0';
			sio << "Read(" << iRead << "): '" << buf << "'";
		}
	}
	else if( argv[1][0] == 'b' )
	{
		sio << "Broadcasting..." << sio.nl;
		Bu::UdpSocket udp("255.255.255.255", 6688,
				UdpSocket::Write|UdpSocket::Broadcast );

		for(;;)
		{
			udp.write("hello", 5 );
			usleep( 250000 );
		}
	}
	else
	{
		sio << "Options are 'l' for listening and 'b' for broadcasting."
			<< sio.nl;
	}

	return 0;
}


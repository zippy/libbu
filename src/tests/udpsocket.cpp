/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/udpsocket.h"
#include "bu/sio.h"

#include <errno.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/utsname.h>

using namespace Bu;

int main( int argc, char *argv[] )
{
	sio << Fmt::hex(8) << INADDR_ANY << sio.nl << Fmt::hex(8) << inet_addr("0.0.0.0") << sio.nl;
	if( argc == 1 )
	{
		sio << "Options are 'l' for listening and 'b' for broadcasting."
			<< sio.nl;
	}
	else if( argv[1][0] == 'l' )
	{
		sio << "Listening..." << sio.nl;
		Bu::UdpSocket udp( "0.0.0.0", 6688, UdpSocket::Read|UdpSocket::Broadcast );

		for(;;)
		{
			char buf[1501];
			int iRead = udp.read( buf, 1500 );
			if( iRead >= 0 )
			{
				buf[iRead] = '\0';
				sio << "Read(" << iRead << "): '" << buf << "'" << sio.nl;
			}
			else
			{
				sio << "Got " << iRead << ": " << strerror( errno ) << sio.nl;
			}
		}
	}
	else if( argv[1][0] == 'L' )
	{
		sio << "Listening..." << sio.nl;
		Bu::UdpSocket udp( "0.0.0.0", 6688, UdpSocket::Read|UdpSocket::Broadcast );

		for(;;)
		{
			char buf[1501];
			Bu::UdpSocket::addr aHost;
			int iPort;
			int iRead = udp.read( buf, 1500, aHost, iPort );
			if( iRead >= 0 )
			{
				buf[iRead] = '\0';
				sio << "Read(" << iRead << ") from " << Bu::UdpSocket::addrToStr( aHost ) << ":" << iPort << ": '" << buf << "'" << sio.nl;
			}
		}
	}
	else if( argv[1][0] == 'b' )
	{
		sio << "Broadcasting..." << sio.nl;
		Bu::UdpSocket udp("255.255.255.255", 6688,
				UdpSocket::Write|UdpSocket::Broadcast );

		for(;;)
		{
			int iWrote = udp.write("hello", 5 );
			sio << "Wrote(" << iWrote << "): " << strerror( errno ) << sio.nl;
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


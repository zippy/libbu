#include <bu/queuebuf.h>
#include <bu/sio.h>

using namespace Bu;

int main()
{
	static const char *src = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789%#";
	QueueBuf qb;

	for( int j = 0; j < 8; j++ )
	{
		qb.write( src, 60 );
	}

	char buf[11];
	while( !qb.isEos() )
	{
		buf[qb.read( buf, 9 )] = '\0';
		sio << "Read: >>" << buf << "<<" << sio.nl;
	}
}


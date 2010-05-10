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

	char buf[11], bufp[11];
	while( !qb.isEos() )
	{
		int iAmntPeek = qb.peek( bufp, 9 );
		int iAmntRead = qb.read( buf, 9 );
		if( iAmntPeek != iAmntRead )
			sio << "Differ: " << iAmntPeek << " vs " << iAmntRead << sio.nl;
		buf[iAmntRead] = '\0';
		bufp[iAmntPeek] = '\0';
		if( memcmp( buf, bufp, iAmntPeek ) )
		{
			sio << "Buffers differ" << sio.nl
				<< "  " << buf << sio.nl
				<< "  " << bufp << sio.nl;
		}
		else
			sio << "Read: >>" << buf << "<<" << sio.nl;
	}

	sio << sio.nl << sio.nl << sio.nl << "Seek test:" << sio.nl << sio.nl;

	for( int j = 0; j < 5; j++ )
	{
		qb.write( src, 25 );
		qb.write( src+10, 25 );
	}

	char bufa[26];
	char bufb[26];
	::memcpy( bufb, src+10, 15 );
	::memcpy( bufb+15, src+10, 10 );
	bufb[25] = '\0';
	sio << "Comparing to '" << bufb << "'" << sio.nl;
	qb.seek( 10 );
	while( !qb.isEos() )
	{
		bufa[qb.read( bufa, 25 )] = '\0';
		qb.seek( 25 );
		if( memcmp( bufa, bufb, 25 ) )
			sio << "Differ?" << sio.nl;
		sio << "=== '" << bufa << "' == '" << bufb << "'" << sio.nl;
	}
}


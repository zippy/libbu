#include <bu/socket.h>
#include <bu/sio.h>

#include <sys/time.h>
#include <time.h>

using namespace Bu;

bool isUp()
{
	try
	{
		Socket s("xagasoft.com", 9898, 1 );

		char buf[5];
		buf[s.read(buf, 2, 1, 0)] = '\0';
		
		if( !strcmp( buf, "hi" ) )
			return true;
		else
			return false;
	}
	catch(...)
	{
		return false;
	}
}

int main()
{
	uint32_t uUp = 0;
	uint32_t uDown = 0;
	uint32_t uTotal = 0;
	struct timeval tv;

	for(;;)
	{
		gettimeofday( &tv, NULL );
		time_t goal = ((tv.tv_sec/5)+1)*5;
		tv.tv_sec = goal-tv.tv_sec;
		tv.tv_usec = 0-tv.tv_usec;
		if( tv.tv_usec < 0 )
		{
			tv.tv_sec--;
			tv.tv_usec = 1000000+tv.tv_usec;
		}
		select( 0, NULL, NULL, NULL, &tv );
		gettimeofday( &tv, NULL );
		if( isUp() )
		{
			uUp++;
			sio << "status: up   ";
		}
		else
		{
			uDown++;
			sio << "status: down ";
		}
		uTotal++;

		sio << "(up=" << (uUp*5) << "s, down=" << (uDown*5) << ") up for "
			<< uUp*100/uTotal << "% of " << uTotal*5 << "s" << sio.nl
			<< sio.flush;
	}
}


#include "bu/minicron.h"
#include "bu/sio.h"

#include <unistd.h>

using namespace Bu;
	
Bu::MiniCron mCron;

void job0( Bu::MiniCron::Job &job )
{
	sio << time( NULL ) << ": job0( id = " << job.getId() << ", count = "
		<< job.getRunCount() << ")" << sio.nl;
}

void job1( Bu::MiniCron::Job &job )
{
	sio << time( NULL ) << ": job1( id = " << job.getId() << ", count = "
		<< job.getRunCount() << ")" << sio.nl;
	mCron.removeJob( 4 );
}

void job2( Bu::MiniCron::Job &job )
{
	sio << time( NULL ) << ": job2( id = " << job.getId() << ", count = "
		<< job.getRunCount() << ")" << sio.nl;
}

void job3( Bu::MiniCron::Job &job )
{
	sio << time( NULL ) << ": job3( id = " << job.getId() << ", count = "
		<< job.getRunCount() << ")" << sio.nl;
}

int main()
{

	mCron.addJob( slot( &job0 ), MiniCron::TimerInterval( time(NULL)+3, 5 ) );
	mCron.addJob( slot( &job1 ), MiniCron::TimerInterval( time(NULL)+10, 8 ) );
	mCron.addJob( slot( &job2 ), MiniCron::TimerBasic("weekly wed 17") );
	mCron.addJob( slot( &job3 ), MiniCron::TimerInterval( time(NULL)+1, 2 ) );

	sio << time( NULL ) << ": Program started." << sio.nl;

	for(;;)
	{
		usleep( 50000 );
		mCron.poll();
	}

	return 0;
}


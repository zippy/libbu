/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

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
	mCron.addJob(
		"job0", slot( &job0 ), MiniCron::TimerInterval( time(NULL)+3, 5 ) );
	mCron.addJob(
		"job1", slot( &job1 ), MiniCron::TimerInterval( time(NULL)+10, 8 ) );
	mCron.addJob(
		"job2", slot( &job2 ), MiniCron::TimerBasic("weekly wed 17") );
	mCron.addJob(
		"job3", slot( &job3 ), MiniCron::TimerInterval( time(NULL)+1, 2 ) );

	sio << time( NULL ) << ": Program started." << sio.nl;

	for(;;)
	{
		usleep( 50000 );
		mCron.poll();
	}

	return 0;
}


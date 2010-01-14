#include "bu/minicron.h"

#include <stdlib.h>
#include <time.h>

Bu::MiniCron::MiniCron() :
	jidNext( 1 )
{
}

Bu::MiniCron::~MiniCron()
{
}

bool Bu::MiniCron::hasJobs()
{
	return !hJobs.isEmpty();
}

time_t Bu::MiniCron::getNextRun()
{
	if( hasJobs() )
		return hJobs.peek()->getNextRun();
	return -1;
}

void Bu::MiniCron::poll()
{
	time_t tNow = time( NULL );

	while( !hJobs.isEmpty() )
	{
		if( hJobs.peek()->getNextRun() <= tNow )
		{
			Job *pJob = hJobs.dequeue();
			pJob->run();
			if( pJob->bContinue )
			{
				hJobs.enqueue( pJob );
			}
			else
			{
				delete pJob;
			}
		}
		else
		{
			break;
		}
	}
}

Bu::MiniCron::JobId Bu::MiniCron::addJob( Bu::MiniCron::CronSignal sigJob,
		const Bu::MiniCron::Timer &t )
{
	JobId jid = jidNext++;
	Job *pJob = new Job( jid );
	pJob->sigJob = sigJob;
	pJob->pTimer = t.clone();
	pJob->tNextRun = pJob->pTimer->nextTime();
	hJobs.enqueue( pJob );

	return jid;
}

Bu::MiniCron::JobId Bu::MiniCron::addJobOnce( Bu::MiniCron::CronSignal sigJob,
		const Bu::MiniCron::Timer &t )
{
	JobId jid = jidNext++;
	Job *pJob = new Job( jid, false );
	pJob->sigJob = sigJob;
	pJob->pTimer = t.clone();
	pJob->tNextRun = pJob->pTimer->nextTime();
	hJobs.enqueue( pJob );

	return jid;
}

void Bu::MiniCron::removeJob( JobId jid )
{
	Bu::List<Job *> lJobs;
	while( !hJobs.isEmpty() )
	{
		Job *pJob = hJobs.dequeue();
		if( pJob->getId() == jid )
		{
			delete pJob;
		}
		else
			lJobs.append( pJob );
	}

	for( Bu::List<Job *>::iterator i = lJobs.begin(); i; i++ )
	{
		hJobs.enqueue( *i );
	}
}

Bu::MiniCron::Job::Job( JobId jid, bool bRepeat ) :
	pTimer( NULL ),
	bContinue( bRepeat ),
	jid( jid ),
	tAdded( time( NULL ) ),
	iRunCount( 0 )
{
}

Bu::MiniCron::Job::~Job()
{
	delete pTimer;
	pTimer = NULL;
}

void Bu::MiniCron::Job::run()
{
	iRunCount++;
	tNextRun = pTimer->nextTime();
	sigJob( *this );
}

time_t Bu::MiniCron::Job::getNextRun()
{
	return tNextRun;
}

void Bu::MiniCron::Job::calcNextRun()
{
	if( pTimer )
		tNextRun = pTimer->nextTime();
}

void Bu::MiniCron::Job::setTimer( const Timer &t )
{
	delete pTimer;
	pTimer = t.clone();
}

void Bu::MiniCron::Job::stop()
{
	bContinue = false;
}

void Bu::MiniCron::Job::resume()
{
	bContinue = true;
}

Bu::MiniCron::JobId Bu::MiniCron::Job::getId()
{
	return jid;
}

time_t Bu::MiniCron::Job::getTimeCreated()
{
	return tAdded;
}

int Bu::MiniCron::Job::getRunCount()
{
	return iRunCount;
}

Bu::MiniCron::Timer::Timer()
{
}

Bu::MiniCron::Timer::~Timer()
{
}

Bu::MiniCron::TimerInterval::TimerInterval( time_t tFirst, time_t tInterval ) :
	tNext( tFirst ),
	tInterval( tInterval )
{
}

Bu::MiniCron::TimerInterval::~TimerInterval()
{
}

time_t Bu::MiniCron::TimerInterval::nextTime()
{
	time_t tRet = tNext;
	tNext += tInterval;
	return tRet;
}

Bu::MiniCron::TimerBasic::TimerBasic( const Bu::FString &s ) :
	tLast( -1 ),
	sSpec( s )
{
}

Bu::MiniCron::TimerBasic::~TimerBasic()
{
}

time_t Bu::MiniCron::TimerBasic::nextTime()
{
	if( tLast == -1 )
		tLast = time( NULL );

	Bu::FString::const_iterator i = sSpec.begin();
	switch( lex( i ) )
	{
		case tokDaily:
			{
				int iHour = lexInt( i );
				int iMin = lexInt( i );

				struct tm t;
				::memcpy( &t, localtime( &tLast ), sizeof(struct tm) );
				if( iHour < t.tm_hour ||
					(iHour == t.tm_hour && iMin <= t.tm_min) )
				{
					t.tm_mday++;
				}
				t.tm_hour = iHour;
				t.tm_min = iMin;
				t.tm_sec = 0;
				tLast = mktime( &t );
			}
			break;

		case tokHourly:
			{
				int iMin = lexInt( i );

				struct tm t;
				::memcpy( &t, localtime( &tLast ), sizeof(struct tm) );
				if( iMin <= t.tm_min )
					t.tm_hour++;
				t.tm_min = iMin;
				t.tm_sec = 0;
				tLast = mktime( &t );
			}
			break;

		case tokWeekly:
			{
				int iDay = lexInt( i );
				int iHour = lexInt( i );
				int iMin = lexInt( i );
				
				struct tm t;
				::memcpy( &t, localtime( &tLast ), sizeof(struct tm) );
				if( iDay < t.tm_wday || 
					(iDay == t.tm_wday && iHour < t.tm_hour) ||
					(iDay == t.tm_wday && iHour == t.tm_hour
					 && iMin <= t.tm_min) )
				{
					if( iDay <= t.tm_wday )
						t.tm_mday += 7 - (t.tm_wday-iDay);
					else
						t.tm_mday += 7 - (iDay-t.tm_wday);
				}
				else
				{
					t.tm_mday += (iDay-t.tm_wday);
				}
				t.tm_hour = iHour;
				t.tm_min = iMin;
				t.tm_sec = 0;
				tLast = mktime( &t );
			}
			break;

		case tokMonthly:
			break;

		case tokYearly:
			break;

		default:
			break;
	}

	return tLast;
}

Bu::MiniCron::TimerBasic::Token Bu::MiniCron::TimerBasic::lex(
		Bu::FString::const_iterator &i )
{
	if( !i )
	{
		return tokEos;
	}

	Bu::FString::const_iterator b = i;

	for(; b && (*b == ' ' || *b == '\t'); b++ ) { i = b+1; }
	for(; b && *b != ' ' && *b != '\t'; b++ ) { }

	Bu::FString sTok( i, b );
	i = b;

	if( sTok == "daily" )
		return tokDaily;
	else if( sTok == "hourly" )
		return tokHourly;
	else if( sTok == "weekly" )
		return tokWeekly;
	else if( sTok == "monthly" )
		return tokMonthly;
	else if( sTok == "yearly" )
		return tokYearly;
	else if( sTok == "sun" )
	{
		iVal = 0;
		return valInt;
	}
	else if( sTok == "mon" )
	{
		iVal = 1;
		return valInt;
	}
	else if( sTok == "tue" )
	{
		iVal = 2;
		return valInt;
	}
	else if( sTok == "wed" )
	{
		iVal = 3;
		return valInt;
	}
	else if( sTok == "thu" )
	{
		iVal = 4;
		return valInt;
	}
	else if( sTok == "fri" )
	{
		iVal = 5;
		return valInt;
	}
	else if( sTok == "sat" )
	{
		iVal = 6;
		return valInt;
	}
	else if( sTok[0] >= '0' && sTok[0] <= '9' )
	{
		iVal = strtol( sTok.getStr(), NULL, 0 );
		return valInt;
	}

	return tokErr;
}

int Bu::MiniCron::TimerBasic::lexInt( Bu::FString::const_iterator &i )
{
	Token t = lex( i );
	if( t == tokEos )
		return 0;
	if( t != valInt )
		throw Bu::ExceptionBase("Expected int, got something else.");
	return iVal;
}


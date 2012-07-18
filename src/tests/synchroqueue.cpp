#include <bu/thread.h>
#include <bu/synchroqueue.h>
#include <bu/list.h>

class Thing
{
public:
	Thing( int x ) :
		x( x ),
		y( 0 )
	{
	}

	int x;
	int y;
};

typedef Bu::SynchroQueue<Thing *> ThingQueue;

Bu::Mutex mWorkDone;
int iWorkDone;
Bu::Condition cWorkDone;

void workDone()
{
	mWorkDone.lock();
	iWorkDone--;
	if( iWorkDone == 0 )
	{
		mWorkDone.unlock();
		cWorkDone.lock();
		cWorkDone.signal();
		cWorkDone.unlock();
		return;
	}
	mWorkDone.unlock();
}

class ThingEater : public Bu::Thread
{
public:
	ThingEater( ThingQueue &qThing ) :
		qThing( qThing )
	{
	}

	bool bRunning;

	void setRunning( bool b )
	{
		mRunning.lock();
		bRunning = b;
		mRunning.unlock();
	}

	bool isRunning()
	{
		mRunning.lock();
		bool b = bRunning;
		mRunning.unlock();
		return b;
	}

protected:
	virtual void run()
	{
		setRunning( true );
		while( isRunning() )
		{
			Thing *pThing = qThing.dequeue( 0, 250000 );
			if( pThing == NULL )
				continue;

			pThing->y = pThing->x*2;
			usleep( 10000 );

			workDone();
		}
	}

	ThingQueue &qThing;
	Bu::Mutex mRunning;
};

typedef Bu::List<ThingEater *> ThingEaterList;

int main()
{
	ThingQueue qThing;
	ThingEaterList lEater;
	
	mWorkDone.lock();
	iWorkDone = 1000;
	mWorkDone.unlock();

	for( int j = 0; j < 5; j++ )
		lEater.append( new ThingEater( qThing ) );

	for( ThingEaterList::iterator i = lEater.begin(); i; i++ )
		(*i)->start();

	for( int j = 0; j < 1000; j++ )
	{
		qThing.enqueue( new Thing( j ) );
	}

	mWorkDone.lock();
	mWorkDone.unlock();
	cWorkDone.lock();
	for(;;)
	{
		mWorkDone.lock();
		if( iWorkDone == 0 )
		{
			mWorkDone.unlock();
			break;
		}
		mWorkDone.unlock();
		cWorkDone.wait();
	}
	cWorkDone.unlock();

	for( ThingEaterList::iterator i = lEater.begin(); i; i++ )
		(*i)->setRunning( false );

	for( ThingEaterList::iterator i = lEater.begin(); i; i++ )
		(*i)->join();

	return 0;
}


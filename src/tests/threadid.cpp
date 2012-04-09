#include <bu/thread.h>
#include <bu/sio.h>

#define BU_TRACE
#include <bu/trace.h>

using namespace Bu;

class CopyThing
{
public:
	CopyThing()
	{
		TRACE();
		tidHome = Thread::currentThread();
	}

	CopyThing( const CopyThing &rSrc )
	{
		TRACE();
		tidHome = Thread::currentThread();
		sio << "Same thread?  " << (tidHome == rSrc.tidHome) << sio.nl;
	}

	void doThings()
	{
		TRACE();
		if( tidHome != Thread::currentThread() )
			sio << "Different threads, hard copy here." << sio.nl;
		else
			sio << "Same thread, everything is cool." << sio.nl;
	}

private:
	ThreadId tidHome;
};

class SubThread : public Thread
{
public:
	SubThread( CopyThing &src ) :
		src( src )
	{
		src.doThings();
	}

protected:
	void run()
	{
		src.doThings();
		sio << "run-Child is me? " << (getId() == Thread::currentThread()) << sio.nl;
	}

private:
	CopyThing src;
};

int main( int argc, char *argv[] )
{
	CopyThing a;

	SubThread st( a );
	st.start();

	sio << "Child is me? " << (st.getId() == Thread::currentThread()) << sio.nl;

	st.join();


	return 0;
}


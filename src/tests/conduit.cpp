#include "bu/conduit.h"
#include "bu/sio.h"
#include "bu/ito.h"

using namespace Bu;

class Reader : public Bu::Ito
{
public:
	Reader( Bu::Conduit &rCond ) :
		rCond( rCond )
	{
	}

	virtual ~Reader()
	{
	}

protected:
	virtual void run()
	{
		while( rCond.isOpen() )
		{
			char buf[1025];

			sio << "Reading..." << sio.flush;
			Bu::size iRead = rCond.read( buf, 1024 );
			buf[iRead] = '\0';
			sio << "got " << iRead << " >" << buf << "<" << sio.nl;
		}

		sio << "Conduit closed, exting thread." << sio.nl;
	}

private:
	Bu::Conduit &rCond;
};

int main()
{
	Conduit c;
	Reader r( c );
	r.start();

	sleep( 3 );
	c.write("Hi there", 8 );
	sleep( 3 );
	c.write("Goodbye, soon.", 14 );
	sleep( 3 );
	c.write("...NOW!", 9 );
	c.close();
	sleep( 3 );

	return 0;
}


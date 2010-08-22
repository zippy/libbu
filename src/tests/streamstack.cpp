#include "bu/streamstack.h"

#include "bu/file.h"
#include "bu/base64.h"
#include "bu/bzip2.h"

#include "bu/sio.h"

#include <time.h>

using namespace Bu;

class DoStuff
{
public:
	DoStuff( Bu::Stream &rStream ) :
		rStream( rStream )
	{
	}

	virtual ~DoStuff()
	{
	}

	void write()
	{
		Bu::FString s;
		time_t tNow = time( NULL );
		s = ctime( &tNow );
		long lSize = s.getSize()-1;
		rStream.write( &lSize, sizeof(long) );
		rStream.write( s.getStr(), lSize );
	}

	void read()
	{
		Bu::FString s;
		long lSize;
		rStream.read( &lSize, sizeof(long) );
		s.setSize( lSize );
		rStream.read( s.getStr(), lSize );
		sio << "Read str(" << lSize << ") = '" << s << "'" << sio.nl;
	}

private:
	Bu::Stream &rStream;
};

int main()
{
	Bu::StreamStack ss;

	DoStuff ds( ss );

	try
	{
		ds.write();
		sio << "This shouldn't have worked." << sio.nl;
	}
	catch( Bu::ExceptionBase &e )
	{
		sio << "Got exception, this is good:  " << e.what() << sio.nl;
	}

	ss.setStream( new Bu::File("Hello.test", Bu::File::WriteNew ) );

	ds.write();

	ss.pushFilter<Bu::Base64>();

	ds.write();

	ss.pushFilter<Bu::BZip2>();

	ds.write();

	ss.clearStack();

	ss.setStream( new Bu::File("Hello.test", Bu::File::Read ) );

	ds.read();

	ss.pushFilter<Bu::Base64>();

	ds.read();

	ss.pushFilter<Bu::BZip2>();

	ds.read();

	return 0;
}


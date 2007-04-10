#include "unitsuite.h"
#include "sfile.h"
#include "exceptions.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("SFile");
		addTest( Unit::writeFull );
		addTest( Unit::readBlocks );
		addTest( Unit::readError1 );
		addTest( Unit::readError2 );
	}

	virtual ~Unit() { }

	//
	// Tests go here
	//
	void writeFull()
	{
		Bu::SFile sf("testfile1", "wb");
		for( int c = 0; c < 256; c++ )
		{
			unsigned char ch = (unsigned char)c;
			sf.write( &ch, 1 );
			unitTest( sf.tell() == c+1 );
		}
		//unitTest( sf.canRead() == false );
		//unitTest( sf.canWrite() == true );
		//unitTest( sf.canSeek() == true );
		sf.close();
		struct stat sdat;
		stat("testfile1", &sdat );
		unitTest( sdat.st_size == 256 );
	}

	void readBlocks()
	{
		Bu::SFile sf("testfile1", "rb");
		unsigned char buf[50];
		size_t total = 0;
		for(;;)
		{
			size_t s = sf.read( buf, 50 );
			for( size_t c = 0; c < s; c++ )
			{
				unitTest( buf[c] == (unsigned char)(c+total) );
			}
			total += s;
			if( s < 50 )
			{
				unitTest( total == 256 );
				unitTest( sf.isEOS() == true );
				break;
			}
		}
		sf.close();
	}

	void readError1()
	{
		try
		{
			Bu::SFile sf("doesn'texist", "rb");
			unitFailed("No exception thrown");
		}
		catch( Bu::FileException &e )
		{
			return;
		}
	}

	void readError2()
	{
		Bu::SFile sf("testfile1", "rb");
		char buf[256];
		int r = sf.read( buf, 256 );
		unitTest( r == 256 );
		// You have to read past the end to set the EOS flag.
		unitTest( sf.isEOS() == false );
		try
		{
			sf.read( buf, 5 );
			unitFailed("No exception thrown");
		}
		catch( Bu::FileException &e )
		{
			sf.close();
			return;
		}
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}


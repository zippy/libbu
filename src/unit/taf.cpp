#include "unitsuite.h"
#include "file.h"
#include "tafreader.h"

#include <string.h>
#include <unistd.h>

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("taf");
		addTest( Unit::read1 );
	}

	virtual ~Unit()
	{
	}

	void read1()
	{
#define FN_TMP ("/tmp/tmpXXXXXXXX")
		Bu::FString sFnTmp(FN_TMP);
		Bu::File fOut = Bu::File::tempFile( sFnTmp, "wb" );
		const char *data =
"{test: name=\"Bob\"}"
;
		fOut.write(data,strlen(data));
		fOut.close();

		Bu::File fIn(sFnTmp.c_str(), "rb");
		Bu::TafReader tr(fIn);
		
		Bu::TafGroup *tn = tr.readGroup();
		unitTest( !strcmp("Bob", tn->getProperty("name").c_str()) );
		delete tn;

		unlink(sFnTmp.c_str());
#undef FN_TMP
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}


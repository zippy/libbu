#include "bu/fstring.h"
#include "bu/hash.h"
#include "bu/unitsuite.h"

#include <stdio.h>

class Unit : public Bu::UnitSuite
{
private:
	typedef Bu::Hash<Bu::FString, int> StrIntHash;
public:
	Unit()
	{
		setName("Hash");
		addTest( Unit::test_probe );
	}

	virtual ~Unit()
	{
	}

	void test_probe()
	{
		StrIntHash h;
		char buf[20];
		for(int i=1;i<10000;i++)
		{
			sprintf(buf,"%d",i);
			Bu::FString sTmp(buf);
			h[sTmp] = i;
			unitTest( h.has(sTmp) );
		}
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}


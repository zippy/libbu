#include "fstring.h"
#include "unitsuite.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("FString");
		addTest( Unit::test1 );
	}

	virtual ~Unit()
	{
	}

	void test1()
	{
		unitTest( 1 == 1 );
		unitTest( 1 == 0 );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}


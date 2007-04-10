#include "unitsuite.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("SFile");
		addTest( Unit::test );
	}

	virtual ~Unit() { }

	//
	// Tests go here
	//
	void test()
	{
		unitTest( 1 == 1 );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}


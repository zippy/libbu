#include "fstring.h"
#include "unitsuite.h"

class Unit : public Bu::UnitSuite
{
public:
	Unit()
	{
		setName("FString");
		addTest( Unit::compare1 );
		addTest( Unit::compare2 );
		addTest( Unit::appendSingle );
	}

	virtual ~Unit()
	{
	}

	void compare1()
	{
		Bu::FString b("Bob");
		unitTest( !(b == "Bobo") );
		unitTest( b == "Bob" );
	}
	
	void compare2()
	{
		Bu::FString b("Bobo");
		unitTest( !(b == "Bob") );
		unitTest( b == "Bobo" );
	}

	void appendSingle()
	{
		Bu::FString b;
		for( char l = 'a'; l < 'g'; l++ )
			b += l;
		unitTest( b == "abcdef" );
	}
};

int main( int argc, char *argv[] )
{
	return Unit().run( argc, argv );
}


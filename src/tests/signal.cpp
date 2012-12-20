#include <bu/signals.h>
#include <bu/list.h>
#include <bu/string.h>
#include <bu/sio.h>

using namespace Bu;

void PrintPlain( const Bu::String &s )
{
	println(s);
}

void PrintFancy( const Bu::String &s )
{
	println("STRING!!! ->%1<- !!!GNIRTS").arg( s );
}

int Square( int a )
{
    println("Square called");
	return a*a;
}

int Double( int a )
{
    println("Double called");
	return a+a;
}

int main()
{
	SignalList1<void,const Bu::String &> lPrints;

	lPrints += slot(PrintPlain);
	lPrints += slot(PrintFancy);

	lPrints("Hello there");

	SignalList1<int,int> lMaths;
	lMaths += slot(Double);
	lMaths += slot(Square);
	println("Math'd %1 = %2").arg( 5 ).arg( lMaths(5) );

	return 0;
}


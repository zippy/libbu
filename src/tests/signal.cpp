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
	return a*a;
}

int Double( int a )
{
	return a+a;
}

template<typename R, typename P>
class SignalList1 : public Bu::List<Bu::Signal1<R,P> >
{
	typedef Bu::List<Bu::Signal1<R,P> > MyType;
public:
	SignalList1()
	{
	}

	using MyType::iterator;
	using MyType::const_iterator;

	R operator()( P p1 )
	{
		println("===Non-void um...non-specialization===");
		R tmp;
		for( typename MyType::iterator i = MyType::begin(); i; i++ )
			tmp = (*i)( p1 );
		return tmp;
	}
};

template<typename P>
class SignalList1<void, P> : public Bu::List<Bu::Signal1<void,P> >
{
	typedef Bu::List<Bu::Signal1<void,P> > MyType;
public:
	SignalList1()
	{
	}

	using MyType::iterator;
	using MyType::const_iterator;

	void operator()( P p1 )
	{
		println("===Void specialization===");
		for( typename MyType::iterator i = MyType::begin(); i; i++ )
			(*i)( p1 );
	}
};

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


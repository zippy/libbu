/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include <bu/signals.h>
#include <bu/sio.h>

using namespace Bu;

class Thing
{
public:
	Thing() :
		iState( 82731 )
	{
	}

	virtual ~Thing()
	{
	}

	void fnc0()
	{
		sio << iState << ": void fnc0()" << sio.nl;
	}
	
	void fnc1( int a )
	{
		sio << iState << ": void fnc1( " << a << " )" << sio.nl;
	}
	
	void fnc2( int a, Bu::FString b )
	{
		sio << iState << ": void fnc2( " << a << ", \"" << b << "\" )" << sio.nl;
	}
	
	void fnc3( int a, Bu::FString b, double c )
	{
		sio << iState << ": void fnc3( " << a << ", \"" << b << "\", " << c << " )" << sio.nl;
	}
	
	void fnc4( int a, Bu::FString b, double c, char d )
	{
		sio << iState << ": void fnc4( " << a << ", \"" << b << "\", " << c << ", '" << d << "' )" << sio.nl;
	}
	
	void fnc5( int a, Bu::FString b, double c, char d, long e )
	{
		sio << iState << ": void fnc5( " << a << ", \"" << b << "\", " << c << ", '" << d << "', " << e << " )" << sio.nl;
	}

private:
	int iState;
};

void pfnc0()
{
	sio <<  ": void pfnc0()" << sio.nl;
}

void pfnc1( int a )
{
	sio <<  ": void pfnc1( " << a << " )" << sio.nl;
}

void pfnc2( int a, Bu::FString b )
{
	sio <<  ": void pfnc2( " << a << ", \"" << b << "\" )" << sio.nl;
}

void pfnc3( int a, Bu::FString b, double c )
{
	sio <<  ": void pfnc3( " << a << ", \"" << b << "\", " << c << " )" << sio.nl;
}

void pfnc4( int a, Bu::FString b, double c, char d )
{
	sio <<  ": void pfnc4( " << a << ", \"" << b << "\", " << c << ", '" << d << "' )" << sio.nl;
}

void pfnc5( int a, Bu::FString b, double c, char d, long e )
{
	sio <<  ": void pfnc5( " << a << ", \"" << b << "\", " << c << ", '" << d << "', " << e << " )" << sio.nl;
}

void callit( Signal0<void> sig )
{
	sig();
}

int main()
{
	Thing t;

	Signal0<void> cb0( slot( &t, &Thing::fnc0 ) );
	cb0();
	cb0 = slot( &pfnc0 );
	cb0();
	
	Signal1<void, int> cb1( slot( &t, &Thing::fnc1 ) );
	cb1( 5 );
	cb1 = slot( &pfnc1 );
	cb1( 5 );
	
	Signal2<void, int, Bu::FString> cb2( slot( &t, &Thing::fnc2 ) );
	cb2( 5, "Hi there" );
	cb2 = slot( &pfnc2 );
	cb2( 5, "Hi there" );
	
	Signal3<void, int, Bu::FString, double> cb3( slot( &t, &Thing::fnc3 ) );
	cb3( 5, "Hi there", 12.85 );
	cb3 = slot( &pfnc3 );
	cb3( 5, "Hi there", 12.85 );
	
	Signal4<void, int, Bu::FString, double, char> cb4( slot( &t, &Thing::fnc4 ) );
	cb4( 5, "Hi there", 12.85, 'z' );
	cb4 = slot( &pfnc4 );
	cb4( 5, "Hi there", 12.85, 'z' );
	
	Signal5<void, int, Bu::FString, double, char, long> cb5( slot( &t, &Thing::fnc5 ) );
	cb5( 5, "Hi there", 12.85, 'z', 849 );
	cb5 = slot( &pfnc5 );
	cb5( 5, "Hi there", 12.85, 'z', 849 );

//	Signal1<int, int> cb1( slot( &t, &Thing::fnc1 ) );
//	sio << "Result: " << cb1( 5 ) << sio.nl;
}


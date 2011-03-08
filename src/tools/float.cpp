#include <bu/sio.h>
#include <bu/string.h>
#include <stdlib.h>
#include <math.h>

using namespace Bu;

Bu::String ftos( double dNum )
{
	if( dNum == 1.0 )
		return "0x1p+0";
	if( dNum == 0.0 )
		return signbit(dNum)?"-0x0p+0":"0x0p+0";
	if( dNum == -1.0 )
		return "-0x1p+0";
	Bu::String s("0x1.");
	if( signbit(dNum) )
	{
		s.prepend("-");
		dNum = -dNum;
	}
/*	
	for( int j = 0; j < 150 && dNum; j++ )
	{
		dNum = dNum*2;
		sio << dNum << " -- " << (int)dNum << sio.nl;
		if( (int)dNum == 0 )
			s += '0';
		else
		{
			s += '1';
			dNum -= 1.0;
		}
	}
	return s;
	*/
	int iPower = 0;
	if( dNum < 1.0 )
	{
		while( dNum < 1.0 )
		{
			dNum *= 2.0;
			iPower--;
		}
	}
	else if( dNum >= 2.0 )
	{
		while( dNum >= 2.0 )
		{
			dNum *= 0.5;
			iPower++;
		}
	}
	dNum -= (int)dNum;
	for( int j = 0; j < 150 && dNum; j++ )
	{
		dNum = dNum*16.0;
		s += "0123456789abcdef"[(int)dNum];
		dNum -= (int)dNum;
	}
	s.formatAppend("p%+d", iPower );
	return s;
}

double stof( const Bu::String &s )
{
	if( s == "0x1p+0" )
		return 1.0;
	if( s == "-0x1p+0" )
		return -1.0;
	if( s == "0x0p+0" )
		return 0.0;
	if( s == "-0x0p+0" )
		return 0.0;
	double dNum = 0.0;
	int iPPos = s.getSize()-1;
	for(; s[iPPos] != 'p'; iPPos--) { }
	for( int i = iPPos-1; s[i] != '.'; i-- )
	{
		if( s[i]>='0' && s[i]<='9' )
		{
			dNum += s[i]-'0';
		}
		else if( s[i]>='a' && s[i]<='f' )
		{
			dNum += s[i]-'a'+10.0;
		}
		dNum *= 0.0625;
	}

	dNum += 1.0;

	int iTop = strtol( s.getStr()+iPPos+1, NULL, 10 );
	if( iTop < 0 )
	{
		for( int j = 0; j > iTop; j-- )
		{
			dNum *= 0.5;
		}
	}
	else
	{
		for( int j = 0; j < iTop; j++ )
		{
			dNum *= 2.0;
		}
	}

	if( s[0] == '-' )
		return -dNum;
	return dNum;
}

int main( int argc, char *argv[] )
{
	for( int j = 1; j < argc; j++ )
	{
		double dNum = strtod( argv[j], NULL );

		String s = ftos( dNum );
		sio << "in:     " << dNum << sio.nl;
		sio << "mine:   " << s << sio.nl;
		printf("printf: %a\n", dNum );
		sio << "out:    " << stof( s ) << sio.nl;
		sio << "match:  " << (stof( s ) == dNum) << sio.nl << sio.nl;
	}

	return 0;
}


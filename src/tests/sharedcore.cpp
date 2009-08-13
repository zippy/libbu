#include "bu/sharedcore.h"
#include "bu/sio.h"

using namespace Bu;

struct ShintCore
{
	int val;
};
class Shint : public Bu::SharedCore<struct ShintCore>
{
public:
	Shint()
	{
		data->val = 0;
	}

	Shint( int val )
	{
		data->val = val;
	}

	int getVal()
	{
		return data->val;
	}

	void setValBad( int val )
	{
		data->val = val;
	}
	
	void setVal( int val )
	{
		_hardCopy();
		data->val = val;
	}

	bool operator==( const Shint &rhs )
	{
		if( data == rhs.data )
		{
			sio << "Same pointer (" << Fmt::ptr() << data << ")" << sio.nl;
			return true;
		}
		if( data->val == rhs.data->val )
		{
			sio << "Same value " << data->val << " ("
				<< Fmt::ptr() << data << " vs "
				<< Fmt::ptr() << rhs.data << ")"
				<< sio.nl;
			return true;
		}
		sio << "Different" << sio.nl;
		return false;
	}
};

#define line( x ) sio << __FILE__ ": " << __LINE__ << ": " << #x << sio.nl; x

int main()
{
	line( Shint a; )
	line( Shint b( 5 ); )

	line( a == b; )

	line( b = a; )
	line( a == b; )

	line( b.setValBad( 12 ); )
	sio << a.getVal() << " != " << b.getVal() << sio.nl;
	line( a == b; )

	line( a.setVal( 3 ); )
	sio << a.getVal() << " != " << b.getVal() << sio.nl;
	line( a == b; )

	line( a.setVal( b.getVal() ); )
	line( a == b; )
}


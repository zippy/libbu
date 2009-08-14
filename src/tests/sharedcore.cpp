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
		core->val = 0;
	}

	Shint( int val )
	{
		core->val = val;
	}

	int getVal() const
	{
		return core->val;
	}

	void setValBad( int val )
	{
		core->val = val;
	}
	
	void setVal( int val )
	{
		_hardCopy();
		core->val = val;
	}

	bool operator==( const Shint &rhs )
	{
		if( core == rhs.core )
		{
			sio << "Same pointer (" << Fmt::ptr() << core << ")" << sio.nl;
			return true;
		}
		if( core->val == rhs.core->val )
		{
			sio << "Same value " << core->val << " ("
				<< Fmt::ptr() << core << " vs "
				<< Fmt::ptr() << rhs.core << ")"
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


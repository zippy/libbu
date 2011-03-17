#include <bu/string.h>
#include <bu/formatter.h>
#include <bu/sio.h>
#include <bu/variant.h>
#include <bu/membuf.h>

using namespace Bu;

class Fmter
{
public:
	Fmter( const Bu::String &sSrc ) :
		sSrc( sSrc )
	{
	}

	template<typename T>
	Fmter &arg( const T &x )
	{
		lParm.append( Pair( x ) );

		return *this;
	}

	template<typename T>
	Fmter &arg( const T &x, Bu::Formatter::Fmt f )
	{
		lParm.append( Pair( x, f ) );

		return *this;
	}

	operator Bu::String() const
	{
		Bu::MemBuf mbOut;
		Bu::Formatter f( mbOut );
		ParmList::const_iterator i = lParm.begin();
		for( Bu::String::const_iterator s = sSrc.begin(); s; s++ )
		{
			if( *s == '%' )
			{
				f << (*i).format << (*i).value;
				i++;
			}
			else
			{
				f << *s;
			}
		}

		return mbOut.getString();
	}

private:
	const Bu::String &sSrc;
	class Pair
	{
	public:
		template<typename T>
		Pair( const T &v ) :
			value( v )
		{
		}

		template<typename T>
		Pair( const T &v, Bu::Formatter::Fmt f ) :
			value( v ),
			format( f )
		{
		}

		Bu::Variant value;
		Bu::Formatter::Fmt format;
	};
	typedef Bu::List<Pair> ParmList;
	ParmList lParm;
};

Bu::Formatter &operator<<( Bu::Formatter &f, const Fmter &r )
{
	return f << (Bu::String)r;
}

int main()
{
	sio << Fmter("A word is % and a number is % %.").arg("Hello").arg(75, Fmt::hex() ).arg(" - ") << sio.nl;
}


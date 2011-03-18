#include <bu/string.h>
#include <bu/formatter.h>
#include <bu/sio.h>
#include <bu/variant.h>
#include <bu/membuf.h>

#include <stdlib.h>

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
		int iCount = lParm.getSize();
		ParmList::const_iterator *aParm =
			new ParmList::const_iterator[iCount];
		{
			int j = 0;
			for( ParmList::const_iterator i = lParm.begin(); i; i++, j++ )
			{
				aParm[j] = i;
			}
		}
		Bu::MemBuf mbOut;
		Bu::Formatter f( mbOut );
		for( Bu::String::const_iterator s = sSrc.begin(); s; s++ )
		{
			if( *s == '%' )
			{
				s++;
				if( *s == '%' )
					f << *s;
				else
				{
					Bu::String sNum;
					while( s && *s >= '0' && *s <= '9' )
					{
						sNum += *s;
						s++;
					}
					int iIndex = strtol( sNum.getStr(), 0, 10 )-1;
					if( iIndex < 0 || iIndex >= iCount )
					{
						delete[] aParm;
						throw Bu::ExceptionBase(
							"Argument index %d is outside of "
							"valid range (1-%d).", iIndex+1, iCount
							);
					}

					f << (*aParm[iIndex]).format << (*aParm[iIndex]).value;
					if( s )
						f << *s;
				}
			}
			else
			{
				f << *s;
			}
		}

		delete[] aParm;
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
	sio << Fmter("A word is %1 and a number is %2 %1").
		arg("Hello").arg(75, Fmt::hex() ).arg(" - ") << sio.nl;
}


#include "archive.h"

Bu::Archive::Archive( Stream &rStream, bool bLoading ) :
	bLoading( bLoading ),
	rStream( rStream )
{
}

Bu::Archive::~Archive()
{
}

void Bu::Archive::write( const void *pData, int32_t nSize )
{
	if( nSize == 0 || pData == NULL )
		return;
	
	rStream.write( (const char *)pData, nSize );
}

void Bu::Archive::read( void *pData, int32_t nSize )
{
	if( nSize == 0 || pData == NULL )
		return;

	rStream.read( (char *)pData, nSize );
}

void Bu::Archive::close()
{
	rStream.close();
}

bool Bu::Archive::isLoading()
{
	return bLoading;
}
Bu::Archive &Bu::Archive::operator<<(bool p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(int8_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(int16_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(int32_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(int64_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(uint8_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(uint16_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(uint32_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(uint64_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(long p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(float p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(double p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(long double p)
{
	write( &p, sizeof(p) );
	return *this;
}

Bu::Archive &Bu::Archive::operator>>(bool &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(int8_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(int16_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(int32_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(int64_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(uint8_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(uint16_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(uint32_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(uint64_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(long &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(float &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(double &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(long double &p)
{
	read( &p, sizeof(p) );
	return *this;
}

Bu::Archive &Bu::Archive::operator&&(bool &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(int8_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(int16_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(int32_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(int64_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(uint8_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(uint16_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(uint32_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(uint64_t &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(float &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(double &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}

Bu::Archive &Bu::Archive::operator&&(long double &p)
{
	if (bLoading)
	{
		return *this >> p;
	}
	else
	{
		return *this << p;
	}
}


Bu::Archive &Bu::operator<<(Bu::Archive &s, Bu::Archival &p)
{
	p.archive( s );
	return s;
}

Bu::Archive &Bu::operator>>(Bu::Archive &s, Bu::Archival &p)
{
	p.archive( s );
	return s;
}

Bu::Archive &Bu::operator<<( Bu::Archive &ar, std::string &s )
{
	ar << (uint32_t)s.length();
	ar.write( s.c_str(), s.length() );

	return ar;
}

Bu::Archive &Bu::operator>>( Bu::Archive &ar, std::string &s )
{
	uint32_t l;
	ar >> l;
	char *tmp = new char[l+1];
	tmp[l] = '\0';
	ar.read( tmp, l );
	s = tmp;
	delete[] tmp;

	return ar;
}


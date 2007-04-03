#include "serializer.h"
#include "serializable.h"
#include <list>

Serializer::Serializer(bool bLoading):
	bLoading(bLoading)
{
}
Serializer::~Serializer()
{
}

bool Serializer::isLoading()
{
	return bLoading;
}
Serializer &Serializer::operator<<(bool p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(int8_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(int16_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(int32_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(int64_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(uint8_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(uint16_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(uint32_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(uint64_t p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(long p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(float p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(double p)
{
	write( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator<<(long double p)
{
	write( &p, sizeof(p) );
	return *this;
}

Serializer &Serializer::operator>>(bool &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(int8_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(int16_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(int32_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(int64_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(uint8_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(uint16_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(uint32_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(uint64_t &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(long &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(float &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(double &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Serializer &Serializer::operator>>(long double &p)
{
	read( &p, sizeof(p) );
	return *this;
}

Serializer &Serializer::operator&&(bool &p)
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

Serializer &Serializer::operator&&(int8_t &p)
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

Serializer &Serializer::operator&&(int16_t &p)
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

Serializer &Serializer::operator&&(int32_t &p)
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

Serializer &Serializer::operator&&(int64_t &p)
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

Serializer &Serializer::operator&&(uint8_t &p)
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

Serializer &Serializer::operator&&(uint16_t &p)
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

Serializer &Serializer::operator&&(uint32_t &p)
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

Serializer &Serializer::operator&&(uint64_t &p)
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

Serializer &Serializer::operator&&(float &p)
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

Serializer &Serializer::operator&&(double &p)
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

Serializer &Serializer::operator&&(long double &p)
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


Serializer &operator<<(Serializer &s, Serializable &p)
{
	p.serialize( s );
	return s;
}

Serializer &operator>>(Serializer &s, Serializable &p)
{
	p.serialize( s );
	return s;
}

Serializer &operator&&(Serializer &s, Serializable &p)
{
	if (s.isLoading())
	{
		return s >> p;
	}
	else
	{
		return s << p;
	}
}

Serializer &operator<<( Serializer &ar, std::string &s )
{
	ar << (uint32_t)s.length();
	ar.write( s.c_str(), s.length() );

	return ar;
}

Serializer &operator>>( Serializer &ar, std::string &s )
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


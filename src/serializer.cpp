#include "serializer.h"
#include "serializable.h"

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

/*Serializer::Serializer &operator&(bool &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(int8_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(int16_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(int32_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(int64_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(uint8_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(uint16_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(uint32_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(uint64_t &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(float &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(double &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}

Serializer::Serializer &operator&(long double &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}*/


Serializer &Serializer::operator<<(Serializable &p)
{
	p.serialize(*this);
	return *this;
}

Serializer &Serializer::operator>>(Serializable &p)
{
	p.serialize(*this);
	return *this;
}

/*Serializer::Serializer &operator&(Serializable &p)
{
	if (bLoading)
	{
		return *this << p;
	}
	else
	{
		return *this >> p;
	}
}*/


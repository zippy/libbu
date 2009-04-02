/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/archive.h"
#include "bu/stream.h"
#include "bu/archival.h"

Bu::Archive::Archive( Stream &rStream, bool bLoading ) :
	bLoading( bLoading ),
	rStream( rStream ),
	nNextID( 1 )
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
Bu::Archive &Bu::Archive::operator<<(char p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(signed char p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(unsigned char p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(signed short p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(unsigned short p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(signed int p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(unsigned int p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(signed long p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(unsigned long p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(signed long long p)
{
	write( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator<<(unsigned long long p)
{
	write( &p, sizeof(p) );
	return *this;
}

Bu::Archive &Bu::Archive::operator>>(bool &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(char &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(signed char &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(unsigned char &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(signed short &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(unsigned short &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(signed int &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(unsigned int &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(signed long &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(unsigned long &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(signed long long &p)
{
	read( &p, sizeof(p) );
	return *this;
}
Bu::Archive &Bu::Archive::operator>>(unsigned long long &p)
{
	read( &p, sizeof(p) );
	return *this;
}
/*
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
*/

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

Bu::Archive &Bu::operator<<(Bu::Archive &ar, class Bu::Archival *p )
{
	ar << *p;
	return ar;
}

Bu::Archive &Bu::operator>>(Bu::Archive &ar, class Bu::Archival *p )
{
	ar >> *p;
	return ar;
}

Bu::Archive &Bu::operator<<( Bu::Archive &ar, std::string &s )
{
	// This should be defined as long anyway, this is just insurance
	ar << (long)s.length();
	ar.write( s.c_str(), s.length() );

	return ar;
}

Bu::Archive &Bu::operator>>( Bu::Archive &ar, std::string &s )
{
	long l;
	ar >> l;
	char *tmp = new char[l+1];
	tmp[l] = '\0';
	ar.read( tmp, l );
	s = tmp;
	delete[] tmp;

	return ar;
}

uint32_t Bu::Archive::getID( const void *ptr )
{
	if( hPtrID.has( (ptrdiff_t)ptr ) )
		return hPtrID.get( (ptrdiff_t)ptr );
	hPtrID.insert( (ptrdiff_t)ptr, nNextID );
	return nNextID++;
}

void Bu::Archive::assocPtrID( void **ptr, uint32_t id )
{
	if( hPtrID.has( id ) )
	{
		*ptr = (void *)hPtrID.get( id );
		return;
	}

	if( !hPtrDest.has( id ) )
		hPtrDest.insert( id, List<void **>() );
			
	hPtrDest[id].getValue().append( ptr );
}

void Bu::Archive::readID( const void *ptr, uint32_t id )
{
	hPtrID.insert( id, (ptrdiff_t)ptr );

	if( hPtrDest.has( id ) )
	{
		Bu::List<void **> &l = hPtrDest.get( id );
		for( Bu::List<void **>::iterator i = l.begin(); i != l.end(); i++ )
		{
			*(*i) = (void *)ptr;
		}

		hPtrDest.erase( id );
	}
}


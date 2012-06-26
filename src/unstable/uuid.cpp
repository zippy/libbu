/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/uuid.h"
#include "bu/file.h"
#include "bu/formatter.h"
#include "bu/membuf.h"
#include <string.h>

#ifdef WIN32
#include <rpc.h>
#include <Rpcdce.h>
#endif

Bu::Uuid::Uuid()
{
	clear();
}

Bu::Uuid::Uuid( const Uuid &src )
{
	memcpy( data, src.data, 16 );
}

Bu::Uuid::Uuid( const Bu::String &sSrc )
{
	if( sSrc.getSize() == 16 )
	{
		memcpy( data, sSrc.getStr(), 16 );
	}
	else if( sSrc.getSize() == 36 )
	{
		// Parse it
		set( sSrc );
	}
}

Bu::Uuid::~Uuid()
{
}

Bu::String Bu::Uuid::toRawString() const
{
	return Bu::String( (char *)data, 16 );
}

Bu::String Bu::Uuid::toString() const
{
	Bu::MemBuf mb;
	Bu::Formatter f( mb );

	for( int j = 0; j < 16; j++ )
	{
		if( j == 4 || j == 6 || j == 8 || j == 10 )
			f << '-';
		f << Bu::Fmt::hex(2).caps(false) << (unsigned int)data[j];
	}

	return mb.getString();
}

Bu::String Bu::Uuid::toUrn() const
{
	return "urn:uuid:" + toString();
}

int Bu::Uuid::getVersion()
{
	return (data[6]&((8|4|2|1)<<4))>>4;
}

#define msb( i ) (1<<(7-i))

void Bu::Uuid::clear()
{
	data[7] = msb(0);
}

Bu::Uuid Bu::Uuid::generate( Bu::Uuid::Type eType )
{
	switch( eType )
	{
		case Version1:
		case Version2:
		case Version3:
		case Version4:
		case Version5:
		default:
		case System:
#if defined(linux)
			Bu::File fIn( "/proc/sys/kernel/random/uuid", Bu::File::Read );
			char dat[36];
			fIn.read( dat, 36 );
			Uuid id;
			id.set( dat );
			return id;
#elif defined(WIN32)
			UUID uuid;
			UuidCreate( &uuid );
			Uuid id;
			id.data[0] = ((unsigned char *)&uuid.Data1)[3];
			id.data[1] = ((unsigned char *)&uuid.Data1)[2];
			id.data[2] = ((unsigned char *)&uuid.Data1)[1];
			id.data[3] = ((unsigned char *)&uuid.Data1)[0];
			id.data[4] = ((unsigned char *)&uuid.Data2)[1];
			id.data[5] = ((unsigned char *)&uuid.Data2)[0];
			id.data[6] = ((unsigned char *)&uuid.Data3)[1];
			id.data[7] = ((unsigned char *)&uuid.Data3)[0];
			memcpy( id.data+8, uuid.Data4, 8 );

			return id;
#else
# error We should be using one of the other fallbacks, but your platform is not supported yet.  Sorry.
#endif
	}
}

void Bu::Uuid::set( const Bu::String &sSrc )
{
	const char *dat = sSrc.getStr();
	int iNibble = 0;
	memset( data, 0, 16 );
	for( int j = 0; j < 36; j++ )
	{
		if( dat[j] == '-' )
			continue;
		unsigned char c = (dat[j]>='0'&&dat[j]<='9')?(dat[j]-'0'):(dat[j]-'a'+10);
		data[iNibble/2] |= (iNibble%2==0)?(c<<4):(c);
		iNibble++;
	}
}

bool Bu::Uuid::operator==( const Uuid &rhs ) const
{
	return memcmp( data, rhs.data, 16 ) == 0;
}

Bu::Uuid &Bu::Uuid::operator=( const Uuid &rhs )
{
	memcpy( data, rhs.data, 16 );
	return *this;
}

template<> uint32_t Bu::__calcHashCode<Bu::Uuid>( const Bu::Uuid &k )
{
	return __calcHashCode<String>( k.toRawString() );
}

template<> bool Bu::__cmpHashKeys<Bu::Uuid>( const Bu::Uuid &a, const Bu::Uuid &b )
{
	return a == b;
}

Bu::ArchiveBase &Bu::operator>>( Bu::ArchiveBase &ar, Uuid &u )
{
	ar.read( u.data, 16 );
	return ar;
}

Bu::ArchiveBase &Bu::operator<<( Bu::ArchiveBase &ar, const Uuid &u )
{
	ar.write( u.data, 16 );
	return ar;
}


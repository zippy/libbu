/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "bu/uuid.h"
#include "bu/file.h"
#include "bu/formatter.h"
#include "bu/membuf.h"
#include <string.h>

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

Bu::Uuid Bu::Uuid::gen()
{
	Bu::File fIn( "/proc/sys/kernel/random/uuid", Bu::File::Read );
	char dat[36];
	fIn.read( dat, 36 );
	Uuid id;
	id.set( dat );
	return id;
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

template<> uint32_t Bu::__calcHashCode<Bu::Uuid>( const Bu::Uuid &k )
{
	return __calcHashCode<String>( k.toRawString() );
}

template<> bool Bu::__cmpHashKeys<Bu::Uuid>( const Bu::Uuid &a, const Bu::Uuid &b )
{
	return a == b;
}


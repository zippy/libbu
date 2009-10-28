/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
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

Bu::Uuid::~Uuid()
{
}

Bu::FString Bu::Uuid::toRawString()
{
	return Bu::FString( (char *)data, 16 );
}

Bu::FString Bu::Uuid::toString()
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
	int iNibble = 0;
	Uuid id;
	memset( id.data, 0, 16 );
	for( int j = 0; j < 36; j++ )
	{
		if( dat[j] == '-' )
			continue;
		unsigned char c = (dat[j]>='0'&&dat[j]<='9')?(dat[j]-'0'):(dat[j]-'a'+10);
		id.data[iNibble/2] |= (iNibble%2==0)?(c<<4):(c);
		iNibble++;
	}

	return id;
}


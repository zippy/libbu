/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_ARCHIVE_BASE_H
#define BU_ARCHIVE_BASE_H

#include <stdint.h>
#include <unistd.h>

namespace Bu
{
	class ArchiveBase
	{
	public:
		ArchiveBase();
		virtual ~ArchiveBase();

		virtual void close()=0;
		virtual void write( const void *pData, size_t iLength )=0;
		virtual void read( void *pData, size_t iLength )=0;
		virtual bool isLoading()=0;
	};

	template<typename T> ArchiveBase &operator&&( ArchiveBase &ar, T &dat )
	{
		if( ar.isLoading() )
		{
			return ar >> dat;
		}
		else
		{
			return ar << dat;
		}
	}

	ArchiveBase &operator<<( ArchiveBase &ar, bool p );
	ArchiveBase &operator<<( ArchiveBase &ar, char p );
	ArchiveBase &operator<<( ArchiveBase &ar, signed char p );
	ArchiveBase &operator<<( ArchiveBase &ar, unsigned char p );
	ArchiveBase &operator<<( ArchiveBase &ar, signed short p );
	ArchiveBase &operator<<( ArchiveBase &ar, unsigned short p );
	ArchiveBase &operator<<( ArchiveBase &ar, signed int p );
	ArchiveBase &operator<<( ArchiveBase &ar, unsigned int p );
	ArchiveBase &operator<<( ArchiveBase &ar, signed long p );
	ArchiveBase &operator<<( ArchiveBase &ar, unsigned long p );
	ArchiveBase &operator<<( ArchiveBase &ar, signed long long p );
	ArchiveBase &operator<<( ArchiveBase &ar, unsigned long long p );
	ArchiveBase &operator<<( ArchiveBase &ar, float p );
	ArchiveBase &operator<<( ArchiveBase &ar, double p );
	ArchiveBase &operator<<( ArchiveBase &ar, long double p );
	
	ArchiveBase &operator>>( ArchiveBase &ar, bool &p );
	ArchiveBase &operator>>( ArchiveBase &ar, char &p );
	ArchiveBase &operator>>( ArchiveBase &ar, signed char &p );
	ArchiveBase &operator>>( ArchiveBase &ar, unsigned char &p );
	ArchiveBase &operator>>( ArchiveBase &ar, signed short &p );
	ArchiveBase &operator>>( ArchiveBase &ar, unsigned short &p );
	ArchiveBase &operator>>( ArchiveBase &ar, signed int &p );
	ArchiveBase &operator>>( ArchiveBase &ar, unsigned int &p );
	ArchiveBase &operator>>( ArchiveBase &ar, signed long &p );
	ArchiveBase &operator>>( ArchiveBase &ar, unsigned long &p );
	ArchiveBase &operator>>( ArchiveBase &ar, signed long long &p );
	ArchiveBase &operator>>( ArchiveBase &ar, unsigned long long &p );
	ArchiveBase &operator>>( ArchiveBase &ar, float &p );
	ArchiveBase &operator>>( ArchiveBase &ar, double &p );
	ArchiveBase &operator>>( ArchiveBase &ar, long double &p );

	
};

#endif

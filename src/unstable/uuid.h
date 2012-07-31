/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UUID_H
#define BU_UUID_H

#include "bu/util.h"
#include "bu/string.h"

namespace Bu
{
	class Uuid
	{
	friend Bu::ArchiveBase &operator>>( Bu::ArchiveBase &ar, Uuid &u );
	friend Bu::ArchiveBase &operator<<( Bu::ArchiveBase &ar, const Uuid &u );
	public:
		Uuid();
		Uuid( const Uuid &src );
		Uuid( const Bu::String &sSrc );
		virtual ~Uuid();

		Bu::String toRawString() const;
		Bu::String toString() const;
		Bu::String toUrn() const;

		enum Type
		{
			System,
			Version1,
			Version2,
			Version3,
			Version4,
			Version5,
		};

		int getVersion();

		static Uuid generate( Type eType = System );
		DEPRECATED static Uuid gen() { return generate(); }

		void clear();
		void set( const Bu::String &sSrc );

		bool operator==( const Uuid &rhs ) const;
		bool operator!=( const Uuid &rhs ) const;
		Uuid &operator=( const Bu::String &rhs ) { set( rhs ); return *this; }
		Uuid &operator=( const Uuid &rhs );

	private:
		unsigned char data[16];
	};

	template<typename T>
	uint32_t __calcHashCode( const T &k );

	template<typename T>
	bool __cmpHashKeys( const T &a, const T &b );

	template<> uint32_t __calcHashCode<Uuid>( const Uuid &k );
	template<> bool __cmpHashKeys<Uuid>(
		const Uuid &a, const Uuid &b );

	Bu::ArchiveBase &operator>>( Bu::ArchiveBase &ar, Uuid &u );
	Bu::ArchiveBase &operator<<( Bu::ArchiveBase &ar, const Uuid &u );
};

#endif

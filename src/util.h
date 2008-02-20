/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_UTIL_H
#define BU_UTIL_H

namespace Bu
{
	template<typename item>
	void swap( item &a, item &b )
	{
		item tmp = a;
		a = b;
		b = tmp;
	}

	template<typename item>
	item &min( item &a, item &b )
	{
		return a<b?a:b;
	}
	
	template<typename item>
	item &max( item &a, item &b )
	{
		return a>b?a:b;
	}
	
	template<typename item>
	item &mid( item &a, item &b, item &c )
	{
		return min( max( a, b ), c );
	}
	
	//
	//  Basic comparison functors
	//
	template<typename item>
	struct __basicLTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return a < b;
		}
	};
	
	template<typename item>
	struct __basicGTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return a > b;
		}
	};

	template<typename item>
	struct __basicPtrLTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return *a < *b;
		}
	};
	
	template<typename item>
	struct __basicPtrGTCmp
	{
		bool operator()( const item &a, const item &b )
		{
			return *a > *b;
		}
	};

};

#endif

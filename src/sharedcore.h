/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SHARED_CORE_H
#define BU_SHARED_CORE_H

#include "bu/util.h"
#include "bu/sio.h"

namespace Bu
{
	template<typename Core>
	class SharedCore
	{
		typedef class SharedCore<Core> _SharedType;
	public:
		SharedCore() :
			data( new Core ),
			iRefCount( new int(1) )
		{
		}

		SharedCore( const _SharedType &rSrc ) :
			data( NULL ),
			iRefCount( NULL )
		{
			_softCopy( rSrc );
		}

		virtual ~SharedCore()
		{
			_deref();
		}

		SharedCore &operator=( const SharedCore &rhs )
		{
			_softCopy( rhs );
			return *this;
		}

		int getRefCount() const
		{
			return *iRefCount;
		}

	protected:
		Core *data;
		void _hardCopy()
		{
			if( !data || !iRefCount )
				return;
			sio << "_hardCopy()" << sio.nl;
			Core *copy = new Core( *data );
			_deref();
			data = copy;
			iRefCount = new int( 1 );
		}

	private:
		void _deref()
		{
			sio << "_deref()" << sio.nl;
			if( (--(*iRefCount)) == 0 )
			{
				sio << " --> iRefCount == 0, cleaning up." << sio.nl;
				delete data;
				delete iRefCount;
			}
			else
				sio << " --> iRefCount == " << *iRefCount << sio.nl;
			data = NULL;
			iRefCount = NULL;
		}

		void _incRefCount()
		{
			sio << "_incRefCount()" << sio.nl;
			if( iRefCount && data )
				++(*iRefCount);
			sio << " --> iRefCount == " << *iRefCount << sio.nl;
		}

		void _softCopy( const _SharedType &rSrc )
		{
			sio << "_softCopy()" << sio.nl;
			if( data )
				_deref();
			data = rSrc.data;
			iRefCount = rSrc.iRefCount;
			_incRefCount();
		}

		int *iRefCount;
	};
};

#endif

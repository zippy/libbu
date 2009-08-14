/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SHARED_CORE_H
#define BU_SHARED_CORE_H

#include "bu/util.h"

namespace Bu
{
	template<typename Core>
	class SharedCore
	{
		typedef class SharedCore<Core> _SharedType;
	public:
		SharedCore() :
			core( _allocateCore() ),
			iRefCount( new int(1) )
		{
		}

		SharedCore( const _SharedType &rSrc ) :
			core( NULL ),
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
		Core *core;
		void _hardCopy()
		{
			if( !core || !iRefCount )
				return;
			if( (*iRefCount) == 1 )
				return;
			Core *copy = _copyCore( core );
			_deref();
			core = copy;
			iRefCount = new int( 1 );
		}

		virtual Core *_allocateCore()
		{
			return new Core();
		}

		virtual Core *_copyCore( Core *pSrc )
		{
			return new Core( *pSrc );
		}

		virtual void _deallocateCore( Core *pSrc )
		{
			delete pSrc;
		}

	private:
		void _deref()
		{
			if( (--(*iRefCount)) == 0 )
			{
				_deallocateCore( core );
				delete iRefCount;
			}
			core = NULL;
			iRefCount = NULL;
		}

		void _incRefCount()
		{
			if( iRefCount && core )
				++(*iRefCount);
		}

		void _softCopy( const _SharedType &rSrc )
		{
			if( core )
				_deref();
			core = rSrc.core;
			iRefCount = rSrc.iRefCount;
			_incRefCount();
		}

		int *iRefCount;
	};
};

#endif

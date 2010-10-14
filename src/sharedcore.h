/*
 * Copyright (C) 2007-2010 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SHARED_CORE_H
#define BU_SHARED_CORE_H

#include "bu/util.h"

#include <stdio.h>
#include <stdarg.h>

namespace Bu
{
	template<typename Shell, typename Core>
	class SharedCore
	{
		typedef class SharedCore<Shell, Core> _SharedType;
	public:
		SharedCore() :
			core( NULL ),
			iRefCount( NULL )
		{
			core = _allocateCore();
			iRefCount = new int(1);
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
			if( core == rhs.core )
				return *this;

			_softCopy( rhs );
			return *this;
		}

		int getRefCount() const
		{
			return *iRefCount;
		}

		Shell clone() const
		{
			Shell s( dynamic_cast<const Shell &>(*this) );
			s._hardCopy();
			return s;
		}

		bool isCoreShared( const Shell &rOther ) const
		{
			return rOther.core == core;
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

		/**
		 * Reset core acts like a hard copy, except instead of providing a
		 * standalone copy of the shared core, it provides a brand new core.
		 *
		 * Very useful in functions used to reset the state of an object.
		 */
		void _resetCore()
		{
			if( core )
				_deref();
			core = _allocateCore();
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

#undef fin
#undef fout

#endif

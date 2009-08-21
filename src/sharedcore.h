/*
 * Copyright (C) 2007-2008 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SHARED_CORE_H
#define BU_SHARED_CORE_H

#include "bu/util.h"

#include <stdio.h>
#include <stdarg.h>

void hardlog(const char *fmt, ...);

void fncin( void *base, const char *fn, void *core, int *refcnt, int params, ... );
void fncout( void *base, const char *fn, void *core, int *refcnt, int params, ... );


#define fin( count, ... ) fncin( (void *)this, __FUNCTION__, core, iRefCount, count, ##__VA_ARGS__ )
#define fout( count, ... ) fncout( (void *)this, __FUNCTION__, core, iRefCount, count, ##__VA_ARGS__ )

#define rc  ((iRefCount==NULL)?(-1):(*iRefCount))

namespace Bu
{
	template<typename Core>
	class SharedCore
	{
		typedef class SharedCore<Core> _SharedType;
	public:
		SharedCore() :
			core( NULL ),
			iRefCount( NULL )
		{
			fin( 0 );
			core = _allocateCore();
			iRefCount = new int(1);
			fout( 0 );
		}

		SharedCore( const _SharedType &rSrc ) :
			core( NULL ),
			iRefCount( NULL )
		{
			fin( 1, &rSrc );
			_softCopy( rSrc );
			fout( 1, &rSrc );
		}

		virtual ~SharedCore()
		{
			fin( 0 );
			_deref();
			fout( 0 );
		}

		SharedCore &operator=( const SharedCore &rhs )
		{
			fin( 1, &rhs );
			_softCopy( rhs );
			fout( 1, &rhs );
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
			fin( 0 );
			if( !core || !iRefCount )
			{	fout( 0 );	return; }
			if( (*iRefCount) == 1 )
			{	fout( 0 );	return; }
			Core *copy = _copyCore( core );
			_deref();
			core = copy;
			iRefCount = new int( 1 );
			fout( 0 );
		}

		virtual Core *_allocateCore()
		{
			fin( 0 );
			fout( 0 );
			return new Core();
		}

		virtual Core *_copyCore( Core *pSrc )
		{
			fin( 0 );
			fout( 0 );
			return new Core( *pSrc );
		}

		virtual void _deallocateCore( Core *pSrc )
		{
			fin( 0 );
			fout( 0 );
			delete pSrc;
		}

	private:
		void _deref()
		{
			fin( 0 );
			if( (--(*iRefCount)) == 0 )
			{
				_deallocateCore( core );
				delete iRefCount;
			}
			core = NULL;
			iRefCount = NULL;
			fout( 0 );
		}

		void _incRefCount()
		{
			fin( 0 );
			if( iRefCount && core )
				++(*iRefCount);
			fout( 0 );
		}

		void _softCopy( const _SharedType &rSrc )
		{
			fin( 1, &rSrc );
			if( core )
				_deref();
			core = rSrc.core;
			iRefCount = rSrc.iRefCount;
			_incRefCount();
			fout( 1, &rSrc );
		}

		int *iRefCount;
	};
};

#undef fin
#undef fout

#endif

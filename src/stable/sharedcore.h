/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
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
	/**
	 * A mechanism for creating classes that perform lazy copies.  The concept
	 * behind this is that instead of copying a large object when it is assigned
	 * or passed into a copy constructor we simply copy a pointer internally.
	 * The assumption is that many times when an object is passed by value we
	 * don't really want to keep the object around, we want the recipient to
	 * take ownership without allocating a new object.  This allows that to
	 * happen.
	 *
	 * When used properly this makes object copying essentially free (O(1),
	 * that is) and performs the actual copy when a user tries to modify the
	 * object.
	 *
	 * For example, lets look at something like the getKeys function in
	 * Bu::Hash.  When this function is called it creates a Bu::List of
	 * appropriate type, fills it with keys, and returns it.  This is a good
	 * way for this function to behave, there may be additional issues if the
	 * List object were allocated with new and not on the stack.  However,
	 * returning the List at the end of the function could potentially take
	 * a very long time depending on the size of the list and the type of the
	 * key.  In this case the getKeys function doesn't want ownership of the
	 * List object, and when it returns it, it's local copy will be destroyed.
	 *
	 * However, List inherits from SharedCore, which means that when it is
	 * returned all we do is copy a pointer to the "core" of the list, which
	 * is a very fast operatorion.  For a brief moment, before anyone can do
	 * anything else, there are two objects referencing the core of that single
	 * list.  However, the getKeys() function will destroy it's local copy
	 * before the calling function can use it's new copy.  That means that by
	 * the time the calling function can use it's new List of keys it is the
	 * only one with a reference to the core, and no copy will need to happen.
	 *
	 * Using SharedCore on your own classes is fairly straight forward.  There
	 * are only a couple of steps.  First, break the class into two classes.
	 * Move every variable from the original class (generally everything that's
	 * private) into the new class.  Then make the original class inherit from
	 * SharedCore.  The SharedCore template takes 2 parameters, first is the
	 * class it's inheriting from, second is the new core class.  Now, in your
	 * original class you will have one class variable, a pointer named core.
	 * All of your original variables will be accessable through core.  The next
	 * step is to access everything you used to through core, and to find 
	 * every function that may change data in the core.  At the top of every
	 * function that may change data you want to call _hardCopy().
	 *
	 * That's more or less it.  A more detailed guide will be written soon.
	 * @todo Write a guide for this.
	 */
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

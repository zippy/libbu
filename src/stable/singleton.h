/*
 * Copyright (C) 2007-2011 Xagasoft, All rights reserved.
 *
 * This file is part of the libbu++ library and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef BU_SINGLETON_H
#define BU_SINGLETON_H

#include <stdio.h>

namespace Bu
{
	/**
	 * Provides singleton functionality in a modular sort of way.  Make this the
	 * base class of any other class and you immediately gain singleton
	 * functionality.  Be sure to make your constructor and various functions use
	 * intellegent scoping.  Cleanup and instantiation are performed automatically
	 * for you at first use and program exit.  There are two things that you must
	 * do when using this template, first is to inherit from it with the name of
	 * your class filling in for T and then make this class a friend of your class.
	 *@code
	 * // Making the Single Singleton:
	 * class Single : public Singleton<Single>
	 * {
	 * 		friend class Singleton<Single>;
	 * 	protected:
	 * 		Single();
	 * 		...
	 * };
	 @endcode
	 * You can still add public functions and variables to your new Singleton child
	 * class, but your constructor should be protected (hence the need for the
	 * friend decleration).
	 */
	template <class T>
	class Singleton
	{
	protected:
		/**
		 * Private constructor.  This constructor is empty but has a body so that
		 * you can make your own override of it.  Be sure that you're override is
		 * also protected.
		 */
		Singleton() {};

	private:
		/**
		 * Copy constructor, defined so that you could write your own as well.
		 */
		Singleton( const Singleton& );

	public:
		/**
		 * Get a handle to the contained instance of the contained class.  It is
		 * a reference.
		 *@returns A reference to the contained object.
		 */
		static T &getInstance()
		{
			static T i;
			return i;
		}
	};
}

#endif

#ifndef SINGLETON_H
#define SINGLETON_H

#include <stdio.h>

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
 *@author Mike Buland
 */
template <class T>
class Singleton
{
protected:
	Singleton() {};

private:
	Singleton( const Singleton& );

public:
	static T &getInstance()
	{
		static T i;
		return i;
	}
};

#endif

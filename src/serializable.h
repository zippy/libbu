#ifndef SERIALIZER_H
#define SERIALIZER_H

//#include "serializer.h"

/**
 * The base class for any class you want to serialize.  Simply include this as
 * a base class, implement the purely virtual serialize function and you've got
 * an easily serializable class.
 */
class Serializable
{
public:
	/**
	 * Does nothing, here for completeness.
	 */
	Serializable();

	/**
	 * Here to ensure the deconstructor is virtual.
	 */
	virtual ~Serializable();

	/**
	 * This is the main workhorse of the serialization system, just override and
	 * you've got a serializable class.  A reference to the Serializer archive
	 * used is passed in as your only parameter, query it to discover if you are
	 * loading or saving.
	 * @param ar A reference to the Serializer object to use.
	 */
	virtual void serialize( class Serializer &ar )=0;
};

#endif

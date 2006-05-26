#ifndef XML_EXCEPTION_H
#define XML_EXCEPTION_H

#include <string>
#include "exception.h"
#include <stdarg.h>

/**
 * A generalized Exception base class.  This is nice for making general and
 * flexible child classes that can create new error code classes.
 */
class XmlException : public Exception
{
public:
	/**
	 * Construct an exception with an error code of zero, but with a
	 * description.  The use of this is not reccomended most of the time, it's
	 * generally best to include an error code with the exception so your
	 * program can handle the exception in a better way.
	 * @param sFormat The format of the text.  See printf for more info.
	 */
	XmlException( const char *sFormat, ... ) throw();
	
	/**
	 * 
	 * @param nCode 
	 * @param sFormat
	 */
	XmlException( int nCode, const char *sFormat, ... ) throw();
	
	XmlException( int nCode=0 ) throw();
};

#endif

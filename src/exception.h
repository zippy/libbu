#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>
#include <stdarg.h>

/**
 * A generalized Exception base class.  This is nice for making general and
 * flexible child classes that can create new error code classes.
 */
class Exception : public std::exception
{
public:
	/**
	 * Construct an exception with an error code of zero, but with a
	 * description.  The use of this is not reccomended most of the time, it's
	 * generally best to include an error code with the exception so your
	 * program can handle the exception in a better way.
	 * @param sFormat The format of the text.  See printf for more info.
	 */
	Exception( const char *sFormat, ... ) throw();
	
	/**
	 * 
	 * @param nCode 
	 * @param sFormat
	 */
	Exception( int nCode, const char *sFormat, ... ) throw();
	
	/**
	 * 
	 * @param nCode 
	 * @return 
	 */
	Exception( int nCode=0 ) throw();
	
	/**
	 * 
	 * @return 
	 */
	virtual ~Exception() throw();

	/**
	 * 
	 * @return 
	 */
	virtual const char *what() const throw();
	
	/**
	 * 
	 * @return 
	 */
	int getErrorCode();
	
	/**
	 * 
	 * @param lpFormat 
	 * @param vargs 
	 */
	void setWhat( const char *lpFormat, va_list &vargs );

private:
	char *sWhat;	/**< The text string telling people what went wrong. */
	int nErrorCode;	/**< The code for the error that occured. */
};

#endif

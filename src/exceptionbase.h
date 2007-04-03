#ifndef EXCEPTION_BASE_H
#define EXCEPTION_BASE_H

#include <string>
#include <exception>
#include <stdarg.h>

namespace Bu
{
	/**
	 * A generalized Exception base class.  This is nice for making general and
	 * flexible child classes that can create new error code classes.
	 *
	 * In order to create your own exception class use these two lines.
	 * 
	 * in your header:  subExceptionDecl( NewClassName );
	 * 
	 * in your source:  subExcpetienDef( NewClassName );
	 */
	class ExceptionBase : public std::exception
	{
	public:
		/**
		 * Construct an exception with an error code of zero, but with a
		 * description.  The use of this is not reccomended most of the time,
		 * it's generally best to include an error code with the exception so
		 * your program can handle the exception in a better way.
		 * @param sFormat The format of the text.  See printf for more info.
		 */
		ExceptionBase( const char *sFormat, ... ) throw();
		
		/**
		 * 
		 * @param nCode 
		 * @param sFormat
		 */
		ExceptionBase( int nCode, const char *sFormat, ... ) throw();
		
		/**
		 * 
		 * @param nCode 
		 * @return 
		 */
		ExceptionBase( int nCode=0 ) throw();
		
		/**
		 * 
		 * @return 
		 */
		virtual ~ExceptionBase() throw();

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

		/**
		 *
		 * @param lpText
		 */
		void setWhat( const char *lpText );

	private:
		int nErrorCode;	/**< The code for the error that occured. */
		char *sWhat;	/**< The text string telling people what went wrong. */
	};
}

#define subExceptionDecl( name )											\
class name : public ExceptionBase											\
{																			\
	public:																	\
		name( const char *sFormat, ... ) throw ();							\
		name( int nCode, const char *sFormat, ... ) throw();				\
		name( int nCode=0 ) throw ();										\
};

#define subExceptionDef( name )												\
name::name( const char *lpFormat, ... ) throw() :							\
	ExceptionBase( 0 )														\
{																			\
	va_list ap;																\
	va_start( ap, lpFormat );												\
	setWhat( lpFormat, ap );												\
	va_end( ap );															\
}																			\
name::name( int nCode, const char *lpFormat, ... ) throw() :				\
	ExceptionBase( nCode )													\
{																			\
	va_list ap;																\
	va_start( ap, lpFormat );												\
	setWhat( lpFormat, ap );												\
	va_end( ap );															\
}																			\
name::name( int nCode ) throw() :											\
	ExceptionBase( nCode )													\
{																			\
}

#endif

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>

class Exception : public std::exception
{
public:
	Exception( const char *sFormat, ... ) throw();
	Exception( int nCode, const char *sFormat, ... ) throw();
	virtual ~Exception() throw();

	virtual const char *what() const throw();
	int getErrorCode();

private:
	char *sWhat;
	int nErrorCode;
};

#endif

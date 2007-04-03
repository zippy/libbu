#include <iostream>
#include "exceptions.h"

int main()
{
	try
	{
		throw ExceptionBase( 42, "There was an error on line: %d", __LINE__ );
	}
	catch( ExceptionBase &e )
	{
		std::cout << "Error "<< e.getErrorCode() << ": " << e.what() << "\n";
	}

	throw ExceptionBase( 112, "This exception wasn't caught!");
}

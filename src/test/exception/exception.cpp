#include <iostream>
#include "exception.h"

int main()
{
	try
	{
		throw Exception( 42, "There was an error on line: %d", __LINE__ );
	}
	catch( Exception &e )
	{
		std::cout << "Error "<< e.getErrorCode() << ": " << e.what() << "\n";
	}

	throw Exception( 112, "This exception wasn't caught!");
}

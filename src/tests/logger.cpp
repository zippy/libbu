#include "bu/logger.h"
#include <errno.h>
#include <stdlib.h>

class Thing
{
	public:
		Thing()
		{
			lineLog( 2, "Want a thing?");
		}

		void go( int i )
		{
			lineLog( 1, "GO!!!!");
		}
};

int main()
{
	setLogLevel( 4 );
	setLogFormat("%L: %y-%02m-%02d %h:%02M:%02s %f:%l:%F: %t");
	lineLog( 5, "Hey, error: %s", strerror( errno ) );

	Thing gh;
	gh.go( 6);
}


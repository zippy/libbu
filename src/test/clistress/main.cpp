#include "connection.h"

int main()
{
	Connection c;

	c.open("localhost", 4001 );

	c.appendOutput("w");
	c.writeOutput();

	c.waitForInput( 6, 5, 0 );

	printf("read: %s\n", c.getInput() );

	c.close();

	return 0;
}


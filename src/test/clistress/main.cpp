#include "connection.h"

int main()
{
	Connection c;

	c.open("localhost", 4001 );

	c.appendOutput("d");
	c.writeOutput();

	c.waitForInput( 5, 40, 0 );

	c.close();

	return 0;
}


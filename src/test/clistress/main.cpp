#include "connection.h"

void _waitForLength( Connection &con, int len)
{
	int rlen = con.getInputAmnt();	
	
	if (rlen >= len)
		return;

	int time_left = 5;
	int mic_left = 0;

	while (rlen < len)
	{
		if (time_left == 0 && mic_left == 0)
		{
			throw "Socket Timeout";
		}
		con.readInput(time_left, mic_left, &time_left, &mic_left);
		rlen = con.getInputAmnt();				
	}
}

int main()
{
	Connection c;

	c.open("localhost", 4001 );

	c.appendOutput("d");
	c.writeOutput();

	_waitForLength( c, 40 );

	c.close();

	return 0;
}


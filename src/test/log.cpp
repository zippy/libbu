#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "multilog.h"
#include "multilogtext.h"

void testlog( const char *text );

class Test
{
public:
	Test()
	{
		MultiLineLog( 4, "Test init'd\n");
	}
};

int main()
{
	MultiLog &xLog = MultiLog::getInstance();

	xLog.LineLog( 2, "Hello again");

	MultiLog::getInstance().addChannel(
		new MultiLogText( STDOUT_FILENO, "%02y-%02m-%02d %02h:%02M:%02s: %t" )
		);

	MultiLineLog( MultiLog::LError, "Hi there!");
	Test t;

	testlog("external test");
}


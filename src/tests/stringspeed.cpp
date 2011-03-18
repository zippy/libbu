#include <bu/string.h>

#include <sys/time.h>
#include <stdio.h>

void timeit( void (*func)(), const char *sName, int iIter )
{
	struct timeval ts, te;

	gettimeofday( &ts, NULL );
	for( int j = 0; j < iIter; j++ )
	{
		func();
	}
	gettimeofday( &te, NULL );

	double dTotal = (double)(te.tv_sec-ts.tv_sec) + (double)(te.tv_usec-ts.tv_usec)/1000000.0;
	printf("%10s:  %f spi (%f total)\n", sName, dTotal/iIter, dTotal );
}

void append1()
{
	Bu::String sTmp;
	for( int c = 0; c < 5000; c++ )
	{
		sTmp += (char)(c%256);
	}
}

void append2()
{
	Bu::String sTmp;
	for( int c = 0; c < 5000; c++ )
	{
		sTmp.append( (char)(c%256) );
	}
}

void append3()
{
	Bu::String sTmp;
	for( int c = 0; c < 5000; c++ )
	{
		sTmp += "Test";
	}
}

void append4()
{
	Bu::String sTmp;
	for( int c = 0; c < 5000; c++ )
	{
		sTmp.append("Test");
	}
}

void append5()
{
	Bu::String sTmp, sAdd("Test");
	for( int c = 0; c < 5000; c++ )
	{
		sTmp += sAdd;
	}
}

void append6()
{
	Bu::String sTmp, sAdd("Test");
	for( int c = 0; c < 5000; c++ )
	{
		sTmp.append( sAdd );
	}
}

void prepend1()
{
	Bu::String sTmp;
	for( int c = 0; c < 5000; c++ )
	{
		sTmp.prepend('c');
	}
}

void copy1()
{
	Bu::String sSrc;
	for( int c = 0; c < 1000; c++ )
	{
		sSrc += (char)(c%256);
		Bu::String sTmp = sSrc;
		sTmp.append( '-' );
	}
}

void copy2()
{
	Bu::String sSrc;
	for( int c = 0; c < 1000; c++ )
	{
		sSrc += (char)(c%256);
		Bu::String sTmp = sSrc;
	}
}

void replace1()
{
	Bu::String s("Hey, this is a replacement test, what do you thing of it?");
	s.replace("e", "X").replace("a", "X").replace("what","XXX");
}

int main()
{
	timeit( append1, "Append1", 5000 );
	timeit( append2, "Append2", 5000 );
	timeit( append3, "Append3", 2000 );
	timeit( append4, "Append4", 2000 );
	timeit( append4, "Prepend1", 2000 );
	timeit( copy1, "Copy1", 2000 );
	timeit( copy2, "Copy2", 2000 );
	timeit( replace1, "Replace1", 10000 );
}


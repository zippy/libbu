#include <bu/sio.h>
using namespace Bu;

#include <iostream>
using namespace std;

typedef struct Counter
{
	Counter() : i( 0 )
	{
	}

	int get()
	{
		i++;
		return i-1;
	}

	int i;
} Counter;

template<typename a>
void runtest( a &out )
{
	Counter c;
	out << c.get() << ", " << c.get() << ", " << c.get() << ", " << c.get() << ", " << c.get() << "\n";
}

int main()
{
	runtest( cout );
	runtest( sio );

	return 0;
}


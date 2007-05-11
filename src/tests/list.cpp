#include "bu/list.h"

int main()
{
	Bu::List<int> l;

	l.append( 0 );

	for( int j = 3; j <= 21; j += 3 )
	{
		l.append( j );
		l.prepend( -j );
	}

	for( Bu::List<int>::iterator i = l.begin(); i != l.end(); i++ )
	{
		printf("%d ", *i );
	}

	printf("\n\n");
}


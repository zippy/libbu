#include "bu/list.h"
#include <list>

typedef struct Bob
{
	int nID;
} Bob;

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
	printf("\n");
	for( Bu::List<int>::iterator i = l.begin(); i != l.end(); i++ )
	{
		l.erase( i );
		if( i != l.end() )
			printf("%d ", *i );
	}

	printf("\n\n");

	Bu::List<Bob> lb;
	for( int j = 0; j < 10; j++ )
	{
		Bob b;
		b.nID = j;
		lb.append( b );
	}

	const Bu::List<Bob> rb = lb;

	for( Bu::List<Bob>::const_iterator i = rb.begin(); i != rb.end(); i++ )
	{
		//i->nID += 2;
		//(*i).nID = 4;
		printf("%d ", i->nID );
	}
	printf("\n\n");
}

